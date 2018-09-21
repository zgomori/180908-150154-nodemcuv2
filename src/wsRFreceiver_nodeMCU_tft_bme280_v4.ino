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

#include "WsnCommon.h"
//#include "WsDisplay.h"
#include "thingSpeakUtil.h"
#include "WsnSensorDataCache.h"
#include "WsnTimer.h"

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

WiFiClient client;
WiFiUDP udp;

WsnReceiverConfig cfg;

//uint32_t touchedMillis = millis();

WsnSensorDataCache sensorDataCache; 
WsnTimer wsnTimer;

char charConvBuffer[10];
ThingSpeakUtil tsUtil(client, cfg.thingSpeakAddress);

/*************************- S E T U P -*****************************************/
void setup() {
	Serial.begin(115200);
	delay(100);
	Serial.println("Weather Sensor Network receiver starting");

	readConfig(cfg);

	initRadioRx(cfg);

	initWifi(cfg);

	initBME280();

  initNtpTime(cfg);

	wsnTimer.init(cfg);
	wsnTimer.setTriggerFunction(timerTrigger);

	tft.begin();
	tft.setRotation(0);
  
	//  touch.begin();  // Must be done before setting rotation
	//  touch.setRotation(0);

	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_GREEN,TFT_BLACK);

	tft.setTextDatum(MC_DATUM);
	//tft.drawString("00:00", 120, 40, 7);
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

		displayData(newDataFromNode);
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

void initNtpTime(WsnReceiverConfig &_cfg){
	DSTcorrectedTimeZone = _cfg.timeZone;
	udp.begin(_cfg.localUdpPort);
	setSyncProvider(getNtpTime);
	setSyncInterval(300);
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
	char buff[3];
	tftClock[0] = '\0';
	strcat(tftClock, itoa(hour(), buff, 10));
	strcat(tftClock, ":");
	addLeadingZero(minute(), buff);
	strcat(tftClock, buff);
	
	//Serial.println(tftClock);
	tft.drawString(tftClock, 120, 40, 7);
}

void addLeadingZero(int number, char* buff){
	if(number < 10){
		buff[0] = '0';
		itoa(number, buff+1,10);
	}
	else{ 
		itoa(number, buff,10);
	}
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime(){
	IPAddress ntpServerIP; // NTP server's ip address

	while (udp.parsePacket() > 0) ; // discard any previously received packets
	Serial.println("Transmit NTP Request");
	// get a random server from the pool
	WiFi.hostByName(cfg.ntpServerName, ntpServerIP);
	Serial.print(cfg.ntpServerName);
	Serial.print(": ");
	Serial.println(ntpServerIP);
	sendNTPpacket(ntpServerIP);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];

			//!!!!!!
			unsigned long ret = secsSince1900 - 2208988800UL + DSTcorrectedTimeZone * SECS_PER_HOUR;
			if(isDst(ret)){
				DSTcorrectedTimeZone = cfg.timeZone + 1;
				ret = secsSince1900 - 2208988800UL + DSTcorrectedTimeZone * SECS_PER_HOUR;
				Serial.println("set Daylight Saving Time");
			}
			//!!!!!!
			return ret;
		}
	}
	Serial.println("[ERROR] No NTP Response");
	return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address){
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}

bool isDst(time_t theTime){
	int _month = month(theTime);
	int _day = day(theTime);
	int _dow = weekday(theTime);

	if (_month < 3 || _month > 10)  return false; 
	if (_month > 3 && _month < 10)  return true; 

	int previousSunday = _day - _dow;

	if (_month == 3) return previousSunday >= 25;
	if (_month == 10) return previousSunday < 25;
	return false; // this line never gonna happend
}
