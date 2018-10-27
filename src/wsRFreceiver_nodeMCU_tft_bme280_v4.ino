#include "ESP8266WiFi.h"
#include <WiFiUdp.h>

#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"

/*
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/MonoSpaced24.h>
*/
#include <TFT_eSPI.h>
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <TimeLib.h>
#include "NTPclient.h"

#include "WsnCommon.h"
//#include "WsDisplay.h"
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

/* Adafruit library settings
#define TFT_DC D4
#define TFT_CS D8
//#define TFT_CS D2
#define TOUCH_CS_PIN  D2
*/

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
uint8_t rfPipeNum;
uint32_t localSensorMessageCnt = 0;
//time_t prevDisplay = 0; // when the digital clock was displayed
int prevMinuteDisplay = -1;
int8_t DSTcorrectedTimeZone;


Adafruit_BME280     bme(BME_CS); // hardware SPI
RF24                radio(RADIO_CE_PIN, RADIO_CSN_PIN);
//Adafruit_ILI9341    tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//XPT2046_Touchscreen touch(TOUCH_CS_PIN);

TFT_eSPI tft = TFT_eSPI();  
TftUtil tftUtil(&tft);
WsnGui wsnGUI(&tft); 
WsnSystemStatus sysStat = WsnSystemStatus();

WiFiClient client;
WiFiUDP udp;
NTPclient ntpClient;

WsnReceiverConfig cfg;

//uint32_t touchedMillis = millis();

WsnSensorDataCache sensorDataCache; 
WsnTimer wsnTimer;

char charConvBuffer[10];
ThingSpeakUtil tsUtil(client, cfg.thingSpeakAddress);

// remove!!!
uint16_t colorPalette16[16];


time_t getNtpTime(){
	return ntpClient.getTime();
}
/*************************- S E T U P -*****************************************/
void setup() {
	Serial.begin(115200);
	delay(100);
	Serial.println("Weather Sensor Network receiver starting");

	readConfig(cfg);

	initWifi(cfg);

	ntpClient.init(udp, cfg.ntpServerName, cfg.localUdpPort, cfg.timeZone);
	setSyncProvider(getNtpTime);
	setSyncInterval(300);

	initRadioRx(cfg);

	initBME280();

	wsnTimer.init(cfg);
	wsnTimer.setTriggerFunction(timerTrigger);

	tft.begin();
	tft.setRotation(0);
  
	//  touch.begin();  // Must be done before setting rotation
	//  touch.setRotation(0);

	tft.fillScreen(TFT_BLACK);
//	tft.setTextColor(TFT_GREEN,TFT_BLACK);
	tft.setTextColor(TFT_DARKGREY,TFT_BLACK);

	tft.setTextDatum(MC_DATUM);


	wsnGUI.drawBackground();

	sysStat.set(sysStat.WIFI, true);
	sysStat.set(sysStat.RADIO, false);
	sysStat.set(sysStat.LOCAL_SENSOR, true);
	sysStat.set(sysStat.NTP, false);
	sysStat.set(sysStat.TS_UPDATE, true);
	sysStat.set(sysStat.TS_GET, false);
	
	wsnGUI.updateStatusBar(sysStat);
}

void loop() {
	newDataFromNode = -1;

	// clock
	if (timeStatus() != timeNotSet) {
		if (minute() != prevMinuteDisplay) { //update the display only if time has changed
			prevMinuteDisplay = minute();
			displayClock();
		}
	}
	yield();

	// read sensors
	getRadioMessage();

	if (newDataFromNode == -1){ 
		if (sensorMillis + 10000L <= millis()){
		  Serial.println("++++10 sec");
		  wsnTimer.ticker(); 
		  sensorMillis = millis();
		}
	 }
	 
	 // process sensor data if arrived

	delay(1);
	if (newDataFromNode > -1){
		showData(newDataFromNode);

		if((newDataFromNode < 6) && (strlen(cfg.thingSpeakAPIKeyArr[newDataFromNode])>0)){
			Serial.println("Update ThingSpeak");
			char updateParams[80] = "\0";
			sensorDataCache.createThingSpeakParam(newDataFromNode, updateParams);
			tsUtil.update(cfg.thingSpeakAPIKeyArr[newDataFromNode] ,updateParams);
		}
		delay(1);

		//displayData(newDataFromNode);
		wsnGUI.displaySensorData(newDataFromNode, sensorDataCache);
		//sensorDataCache.dump();
	 }

	 delay(1);

/*
	 if (touch.touched() && (millis() - touchedMillis > 500)) {
		TS_Point p = touch.getPoint();
		Serial.print("x ="); Serial.print(p.x); Serial.print(" y ="); Serial.println(p.y);
		touchedMillis = millis();
	 }
 */   
}


void getRadioMessage() {
	if ( radio.available(&rfPipeNum)) {
		WsnSensorNodeMessage sensorNodeMessage;
	 	radio.read(&sensorNodeMessage, sizeof(WsnSensorNodeMessage));

		if(sensorNodeMessage.nodeID == rfPipeNum){
			newDataFromNode = sensorNodeMessage.nodeID;
			sensorDataCache.add(sensorNodeMessage);

			Serial.println(F("----------------------------"));
			Serial.print(F("Message received from node "));
			Serial.println(sensorNodeMessage.nodeID);
			Serial.println(F("----------------------------"));
		}
		else{  
			Serial.print(F("[ERROR] getRadioMessage: PipeNumber != nodeID "));
			Serial.print(F("PipeNumber: "));
			Serial.print(rfPipeNum);
			Serial.print(F(" nodeID: "));
			Serial.println(sensorNodeMessage.nodeID);
		}
	}
}

void timerTrigger(int8_t nodeID, int8_t tsNodeConfigIdx){
	Serial.println("-----------------------");
	Serial.print("TRIGGER FIRED ");
	Serial.println(nodeID);
	Serial.println("-----------------------");

	if (nodeID == 0){
		readSensor();
	}
	else{
		char jsonResponse[255];
		char* readApiKey = cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakReadKey;
		char* channel = cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakChannel;
		if(tsUtil.get(readApiKey, channel, jsonResponse)){
			sensorDataCache.add(nodeID, cfg.tsNodeConfigArr[tsNodeConfigIdx].fieldMapping, jsonResponse);
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

void initWifi(WsnReceiverConfig &_cfg){
	WiFi.mode(WIFI_STA); 
	WiFi.begin(_cfg.wifiSsid, _cfg.wifiPass);
	while (WiFi.status() != WL_CONNECTED){
		Serial.println("Waiting for WIFI connection...");
		delay(1000);
	}
	Serial.println("WiFi Connected to SSID.");
	printWifiStatus();
	yield();
}

void readSensor(){
  delay(1);
  WsnSensorNodeMessage _sensorNodeMessage;
  localSensorMessageCnt++;
  bme.takeForcedMeasurement();

  _sensorNodeMessage.nodeID = 0;
  _sensorNodeMessage.sensorSet = cfg.sensorSet;
  _sensorNodeMessage.temperature = bme.readTemperature();
  _sensorNodeMessage.pressure = (bme.readPressure() / 100.0F) + (1.2 * (cfg.elevation/10));
  _sensorNodeMessage.humidity = bme.readHumidity();
  _sensorNodeMessage.batteryVoltage = 0;
  _sensorNodeMessage.messageCnt = localSensorMessageCnt;

  sensorDataCache.add(_sensorNodeMessage);

  delay(1);
}      


void showData(uint8_t nodeID) {
  Serial.println(F("Data received: "));
  Serial.print(F("nodeID:        "));        
  Serial.println(nodeID);
  Serial.print(F("sensorSet:     "));
  Serial.println(sensorDataCache.getSensorSet(nodeID));
  Serial.print(F("temperature:   "));
  Serial.println(sensorDataCache.getTemperature(nodeID));
  Serial.print(F("humidity:      "));
  Serial.println(sensorDataCache.getHumidity(nodeID));
  Serial.print(F("pressure:      "));
  Serial.println(sensorDataCache.getPressure(nodeID));
  Serial.print(F("batteryVoltage:"));
  Serial.println(sensorDataCache.getBatteryVoltage(nodeID));
  Serial.print(F("messageCnt:    "));
  Serial.println(sensorDataCache.getMessageCnt(nodeID));
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

void displayData(uint8_t nodeID){
	if (newDataFromNode == 0){
		tft.setTextDatum(MC_DATUM);
		tft.setTextSize(1);
		tft.drawString(sensorDataCache.getTemperature(0), 60, 140, 6);
		tft.drawString(sensorDataCache.getHumidity(0), 60, 180, 4);
		/*
		tft.setTextPadding(90);  
		tft.setTextDatum(MC_DATUM);
		tft.setFreeFont(CF_OL32);
		tft.drawString(sensorDataCache.getTemperature(0), 60, 120, 1);
		tft.setFreeFont(CF_OL24);
		tft.drawString(sensorDataCache.getHumidity(0), 60, 160, 1);
		*/
	}  
	if (newDataFromNode == 1){
		tft.drawString(sensorDataCache.getTemperature(1), 180, 140, 6);
		tft.drawString(sensorDataCache.getHumidity(1), 180, 180, 4);  
	}  
	if (newDataFromNode == 6){
		tft.drawString(sensorDataCache.getTemperature(6), 60, 240, 6);
		tft.drawString(sensorDataCache.getHumidity(6), 60, 280, 4);
	}  
		if (newDataFromNode == 7){
		tft.drawString(sensorDataCache.getTemperature(7), 180, 240, 6);
		tft.drawString(sensorDataCache.getHumidity(7), 180, 280, 4);
	}  
}

void displayClock(){
	char tftClock[6];
	sprintf(tftClock, "%u:%02u",hour(),minute());
// REMOVE HERE !!!
 	tft.setTextColor(TFT_GREEN,TFT_BLACK);
	tft.setTextDatum(MC_DATUM);  
	tft.drawString(tftClock, 120, 60, 7);
}
