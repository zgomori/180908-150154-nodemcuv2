#include "ESP8266WiFi.h"
#include <WiFiUdp.h>

#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"

#include <TFT_eSPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <TimeLib.h>
#include "NTPclient.h"

#include "WsnCommon.h"
#include "thingSpeakUtil.h"
#include "WsnSensorDataCache.h"
#include "WsnTimer.h"
#include "TftUtil.h"
#include "WsnGui.h"
#include "WsnSystemStatus.h"


#define RADIO_CE_PIN   D3
#define RADIO_CSN_PIN  D0

/*  tft_eSPI library settings*/
#define TFT_CS   PIN_D8  // Chip select control pin D8
#define TFT_DC   PIN_D4  // Data Command control pin
#define TFT_RST  -1  // Reset pin (could connect to NodeMCU RST, see next line)
#define TOUCH_CS PIN_D2     // Chip select pin (T_CS) of touch screen


/*  BME280      NodeMCU
 *  VCC
 *  GND
 *  SCL (CLK)   D5 (GPIO14)
 *  SDA (MOSI)  D7 (GPIO13)
 *  CSB (CS)    D1 
 *  SDO (MISO)  D6 (GPIO12)
 */

#define BME_CS D1

uint32_t sensorMillis = millis();
int8_t newDataFromNode = -1;
uint32_t localSensorMessageCnt = 0;

Adafruit_BME280     bme(BME_CS); // hardware SPI
RF24                radio(RADIO_CE_PIN, RADIO_CSN_PIN);

TFT_eSPI tft = TFT_eSPI();  
uint16_t touchCalibrateData[5] =  { 213, 3571, 377, 3516, 4 };
uint32_t touchedMillis = millis();

TftUtil tftUtil(&tft);
WsnGui wsnGUI(&tft); 
WsnSystemStatus sysStatus = WsnSystemStatus();

WiFiClient client;
WiFiUDP udp;
NTPclient ntpClient;

WsnReceiverConfig cfg;


WsnSensorDataCache sensorDataCache; 
WsnTimer wsnTimer;

ThingSpeakUtil tsUtil(&client, cfg.thingSpeakAddress);


time_t getNtpTime(){
	time_t ret = 0;
	if (checkWifiConnection(cfg)){
		ret = ntpClient.getTime();
		sysStatus.set(sysStatus.NTP, (ret == 0 ? false : true)); 
		sysStatus.set(sysStatus.WIFI, true);
	}	
	else{
		sysStatus.set(sysStatus.WIFI, false);
	}
	// TODO - nem biztos, hogy itt jó helyen van.
	wsnGUI.updateStatusBar(true);

	return ret;
}
/*************************- S E T U P -*****************************************/
void setup() {
	Serial.begin(115200);
	delay(100);
	Serial.println("starting Weather Sensor Network receiver");

	tft.begin();
	tft.setRotation(0);
	tft.setTouch(touchCalibrateData);
  	
	wsnGUI.initMainScreen();

	readConfig(cfg);

	checkWifiConnection(cfg);

	ntpClient.init(udp, cfg.ntpServerName, cfg.localUdpPort, cfg.timeZone);
	setSyncProvider(getNtpTime);
	setSyncInterval(300);

	initRadioRx(cfg);

	initBME280();

	wsnTimer.init(cfg);
	wsnTimer.setTriggerFunction(timerTrigger);


	//  touch.begin();  // Must be done before setting rotation
	//  touch.setRotation(0);
}

void loop() {
	newDataFromNode = -1;

	// dispay clock & Date
	if (wsnGUI.getCurrentScreenId() == wsnGUI.SCR_MAIN){	
		wsnGUI.displayDateTime();
	}
	
	yield();

	// read Radio and store data 
	getRadioMessage();

	yield();
	
	// read local sensor & thingspeak sensors and store data 
	if (newDataFromNode == -1){ 
		wsnTimer.ticker();
		yield(); 
	}
	
	 // process sensor data if arrived

	delay(1);
	if (newDataFromNode > -1){
		sensorDataCache.printData(newDataFromNode);

		// send radio and local sensor data to thingspeak 
		if((newDataFromNode < 6) && (strlen(cfg.thingSpeakAPIKeyArr[newDataFromNode])>0)){
			updateThingSpeak(newDataFromNode);	
		}
	 }

	yield();
	// refresh data on TFT screen
	if (newDataFromNode > -1){ 
		if (wsnGUI.getCurrentScreenId() == wsnGUI.SCR_MAIN){
			wsnGUI.displaySensorData(newDataFromNode);
			wsnGUI.updateStatusBar(true);
		}
		if (wsnGUI.getCurrentScreenId() == wsnGUI.SCR_MENU){
			wsnGUI.displaySensorDump();
		}		
		//sensorDataCache.dump();
	}

	 delay(1);

	// touch handler
	uint16_t touchX = 0, touchY = 0; 
	if (tft.getTouch(&touchX, &touchY) && (millis() - touchedMillis > 500)) {
		touchedMillis = millis(); 
		tft.fillCircle(touchX, touchY, 2, TFT_WHITE);
		Serial.print("x,y = ");
		Serial.print(touchX);
		Serial.print(",");
		Serial.println(touchY);

		if (wsnGUI.getCurrentScreenId() == wsnGUI.SCR_MAIN){
			wsnGUI.switchScreen(wsnGUI.SCR_MENU);
		}
		else{
			wsnGUI.switchScreen(wsnGUI.SCR_MAIN);
		}
	}

}


void getRadioMessage() {
	uint8_t rfPipeNum;
	if ( radio.available(&rfPipeNum)) {
		WsnSensorNodeMessage sensorNodeMessage;
	 	radio.read(&sensorNodeMessage, sizeof(WsnSensorNodeMessage));

		if(sensorNodeMessage.nodeID == rfPipeNum){
			newDataFromNode = sensorNodeMessage.nodeID;
			sensorDataCache.add(sensorNodeMessage);
			sysStatus.set(sysStatus.RADIO, true);

			Serial.println(F("----------------------------"));
			Serial.print(F("Message received from node "));
			Serial.println(sensorNodeMessage.nodeID);
			Serial.println(F("----------------------------"));
		}
		else{  
			sysStatus.set(sysStatus.RADIO, false);
			Serial.print(F("[ERROR] getRadioMessage: PipeNumber != nodeID "));
			Serial.print(F("PipeNumber: "));
			Serial.print(rfPipeNum);
			Serial.print(F(" nodeID: "));
			Serial.println(sensorNodeMessage.nodeID);
		}
	}
}

void timerTrigger(int8_t nodeID, int8_t tsNodeConfigIdx){
	bool sensorReadStatus; 
	Serial.println("-----------------------");
	Serial.print("TRIGGER FIRED ");
	Serial.println(nodeID);
	Serial.println("-----------------------");

	if (nodeID == 0){
		sensorReadStatus = readLocalBME280();
		sysStatus.set(sysStatus.LOCAL_SENSOR, sensorReadStatus);
	}
	else{
		if (checkWifiConnection(cfg)){
			char jsonResponse[255];
			char* readApiKey = cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakReadKey;
			char* channel = cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakChannel;
			bool tsGetStatus = tsUtil.get(readApiKey, channel, jsonResponse); 
			if(tsGetStatus){
				sensorDataCache.add(nodeID, cfg.tsNodeConfigArr[tsNodeConfigIdx].fieldMapping, jsonResponse);
				sysStatus.set(sysStatus.TS_GET, true);
			}
			else{
				sysStatus.set(sysStatus.TS_GET, false);	
			}
			sysStatus.set(sysStatus.WIFI, true);
		}
		else{
			sysStatus.set(sysStatus.WIFI, false);
		}	
		delay(100);
	}
	newDataFromNode = nodeID;
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("physical address: ");
  Serial.println(WiFi.BSSIDstr().c_str());
}


void readConfig(WsnReceiverConfig &_cfg){  
  _cfg.radioNetworkAddress = 0xA0A0A0FFLL;
  _cfg.radioChannel = 101;
  strcpy(_cfg.wifiSsid, "wxIoT");
  strcpy(_cfg.wifiPass,"tXgbYPy6DzYaO-U4");

	strcpy(_cfg.ntpServerName, "time.google.com");
	_cfg.timeZone = 1;
	_cfg.localUdpPort = 8760; 

  strcpy(_cfg.thingSpeakAPIKeyArr[0],"JXWWMBZMQZNRMOJK");  
  strcpy(_cfg.thingSpeakAPIKeyArr[1],"5LXV4LVUS2D6OEJA");
  strcpy(_cfg.thingSpeakAPIKeyArr[2],"5LXV4LVUS2D6OEJA");
  _cfg.sensorSet = 23;
  _cfg.sensorReadFrequencyMs = 60000L;
  _cfg.elevation = 126;

  strcpy(_cfg.tsNodeConfigArr[0].name, "Peti");
  strcpy(_cfg.tsNodeConfigArr[0].thingSpeakReadKey, "9ZTPTLMLNFU8VZU3");
  strcpy(_cfg.tsNodeConfigArr[0].thingSpeakChannel, "340091");
  _cfg.tsNodeConfigArr[0].fieldMapping[WSN_TEMPERATURE] = 1;
  _cfg.tsNodeConfigArr[0].fieldMapping[WSN_HUMIDITY] = 2;
  _cfg.tsNodeConfigArr[0].nodeID = 6;
  _cfg.tsNodeConfigArr[0].readFrequencyMs = 61000L;

  strcpy(_cfg.tsNodeConfigArr[1].name, "Central");
  strcpy(_cfg.tsNodeConfigArr[1].thingSpeakReadKey, "JXWWMBZMQZNRMOJK");
  strcpy(_cfg.tsNodeConfigArr[1].thingSpeakChannel, "528401");
  _cfg.tsNodeConfigArr[1].fieldMapping[WSN_TEMPERATURE] = 1;
  _cfg.tsNodeConfigArr[1].fieldMapping[WSN_HUMIDITY] = 2;
  _cfg.tsNodeConfigArr[1].fieldMapping[WSN_PRESSURE] = 3;
  _cfg.tsNodeConfigArr[1].fieldMapping[WSN_MESSAGES] = 4;
  _cfg.tsNodeConfigArr[1].nodeID = 7;
  _cfg.tsNodeConfigArr[1].readFrequencyMs = 60000L;

}

void initRadioRx(WsnReceiverConfig &_cfg){
  radio.begin();
  radio.setChannel(_cfg.radioChannel);    
  radio.setDataRate( RF24_250KBPS );    
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_LOW);

  for(int i=1; i < 6; i++ ){
	 radio.openReadingPipe(i, (_cfg.radioNetworkAddress * 0x100LL)+i);
  }
  radio.startListening();
  yield();
  Serial.println("RF radio details:");
  radio.printDetails();   
}

bool checkWifiConnection(WsnReceiverConfig &_cfg){
	if (WiFi.status() == WL_CONNECTED){
		return true;
	}

	WiFi.mode(WIFI_STA); 
	WiFi.begin(_cfg.wifiSsid, _cfg.wifiPass);

	uint32_t startMillis = millis();
	while ((WiFi.status() != WL_CONNECTED) || (millis() - startMillis > 5000)){
		Serial.println("Connecting to SSID...");
		delay(500);
	}

	if (WiFi.status() == WL_CONNECTED){
		Serial.println("WiFi Connected to SSID.");
		printWifiStatus();
		return true;
	}	
	else{
		Serial.println("WiFi Connection failed.");
		return false;
	}
}

bool readLocalBME280(){
	delay(1);
	WsnSensorNodeMessage _sensorNodeMessage;
	localSensorMessageCnt++;
	bme.takeForcedMeasurement();
	delay(10);

	float temp = bme.readTemperature();
	float pressure = (bme.readPressure() / 100.0F) + (1.2 * (cfg.elevation/10));
	float humidity = bme.readHumidity();

	if ((temp == 0.0) && (pressure = 0.0) && (humidity = 0.0)){
		return false;
	} 
	_sensorNodeMessage.nodeID = 0;
	_sensorNodeMessage.sensorSet = cfg.sensorSet;
	_sensorNodeMessage.temperature = temp;
	_sensorNodeMessage.pressure = pressure;
	_sensorNodeMessage.humidity = humidity;
	_sensorNodeMessage.batteryVoltage = 0;
	_sensorNodeMessage.messageCnt = localSensorMessageCnt;

	sensorDataCache.add(_sensorNodeMessage);

	return true;
}      

void initBME280(){
	bool status = bme.begin();  
	if (!status) {
		Serial.println(F("[ERROR] Could not find a valid BME280 sensor, check wiring!"));
		while (1);
	}

	bme.setSampling(Adafruit_BME280::MODE_FORCED,
		Adafruit_BME280::SAMPLING_X1, // temperature
		Adafruit_BME280::SAMPLING_X1, // pressure
		Adafruit_BME280::SAMPLING_X1, // humidity
		Adafruit_BME280::FILTER_OFF   
	);
}

void updateThingSpeak(int8_t nodeID){
	if (checkWifiConnection(cfg)){
		Serial.println("Update ThingSpeak");
		char updateParams[80] = "\0";
		sensorDataCache.createThingSpeakParam(nodeID, updateParams);
		bool tsUpdateStat = tsUtil.update(cfg.thingSpeakAPIKeyArr[nodeID], updateParams);
		sysStatus.set(sysStatus.WIFI, true);
		sysStatus.set(sysStatus.TS_UPDATE, tsUpdateStat);
	}
	else{
		sysStatus.set(sysStatus.WIFI, false);
	}
}