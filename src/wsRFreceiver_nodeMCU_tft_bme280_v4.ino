#include "ESP8266WiFi.h"  

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

Adafruit_BME280     bme(BME_CS); // hardware SPI
RF24                radio(RADIO_CE_PIN, RADIO_CSN_PIN);
//Adafruit_ILI9341    tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//XPT2046_Touchscreen touch(TOUCH_CS_PIN);

TFT_eSPI tft = TFT_eSPI();  




WiFiClient client;

WsReceiverConfig cfg;

//uint32_t touchedMillis = millis();
uint32_t sensorMillis = millis();

int8_t newDataFromNode = -1;
uint8_t rfPipeNum;
uint32_t localSensorMessageCnt = 0;

WsnSensorDataCache sensorDataCache; 
WsnTimer wsnTimer;

char charConvBuffer[10];
WsSensorNodeMessage sensorNodeMessage;
ThingSpeakUtil tsUtil(client, cfg.thingSpeakAddress);

void timerTrigger(int8_t nodeID, int8_t tsNodeConfigIdx){
  Serial.print("TRIGGER FIRED ");
  Serial.println(nodeID);

  if (nodeID == 0){
    readSensor(sensorNodeMessage);
  }
  else{
    char jsonResponse[255];
    tsUtil.get(cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakReadKey, cfg.tsNodeConfigArr[tsNodeConfigIdx].thingSpeakChannel, jsonResponse);
    sensorDataCache.add(nodeID, cfg.tsNodeConfigArr[tsNodeConfigIdx].fieldMapping, jsonResponse);
    delay(100);

  }
  newDataFromNode = nodeID;
}

void setup() {
    // printf_begin();
    Serial.begin(115200);
    delay(100);
    Serial.println("Weather Station receiver starting");
    Serial.println(sizeof(WsSensorNodeData));

    readConfig(cfg);

    initRadioRx(cfg);

    initWifi(cfg);

    wsnTimer.init(cfg);
    wsnTimer.setTriggerFunction(timerTrigger);

    tft.init();

  //  touch.begin();  // Must be done before setting rotation
  //  touch.setRotation(0);

  //  tft.setRotation(0);
  //  tft.fillScreen(ILI9341_WHITE);


  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  
  tft.setTextDatum(MC_DATUM);
  tft.drawString("09:55", 120, 40, 7);

    bool status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );

/*
    char jsonResponse[255];
    tsUtil.get(cfg.tsNodeConfigArr[0].thingSpeakReadKey, cfg.tsNodeConfigArr[0].thingSpeakChannel, jsonResponse);
    sensorDataCache.add(6, cfg.tsNodeConfigArr[0].fieldMapping, jsonResponse);
    delay(100);

    tsUtil.get(cfg.tsNodeConfigArr[1].thingSpeakReadKey, cfg.tsNodeConfigArr[1].thingSpeakChannel, jsonResponse);
    sensorDataCache.add(7, cfg.tsNodeConfigArr[1].fieldMapping, jsonResponse);
    delay(100);

    sensorDataCache.dump();
    delay(100);
*/    
}

void loop() {
    // read sensors
    newDataFromNode = -1;
    getRadioMessage(sensorNodeMessage);

    if (newDataFromNode > -1){
      sensorDataCache.add(sensorNodeMessage);
    }
    else{
      if (sensorMillis + 10000L <= millis()){
        wsnTimer.ticker(); 
        sensorMillis = millis();
      }
    }
    
    // process sensor data if arrived

    delay(1);
    if (newDataFromNode > -1){
      showData(newDataFromNode);

      //delay(1);      
      //updateThingSpeak(newDataFromNode);
      if(cfg.thingSpeakAPIKeyArr[newDataFromNode]){
      	char updateParams[80] = "\0";
      	sensorDataCache.createThingSpeakParam(newDataFromNode, updateParams);
        tsUtil.update(cfg.thingSpeakAPIKeyArr[newDataFromNode] ,updateParams);
      }
      delay(1);


      if (newDataFromNode == 0){
        tft.setTextDatum(MC_DATUM);
        tft.setTextSize(1);
        tft.drawString(sensorDataCache.getTemperature(0), 60, 140, 6);
        tft.setTextSize(2);
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
        tft.setTextSize(1);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(sensorDataCache.getTemperature(1), 180, 140, 6);
        tft.drawString(sensorDataCache.getHumidity(1), 180, 180, 4);  
      /*

        tft.setTextPadding(90);  
        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(CF_OL32);
        tft.drawString(sensorDataCache.getTemperature(1), 180, 120, 1);
        tft.setFreeFont(CF_OL24);
        tft.drawString(sensorDataCache.getHumidity(1), 180, 160, 1);
        */
      }  
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

//==============

void getRadioMessage(WsSensorNodeMessage &_sensorNodeMessage) {
  if ( radio.available(&rfPipeNum) ) {
    radio.read( &_sensorNodeMessage, sizeof(WsSensorNodeMessage) );

    if(_sensorNodeMessage.nodeID == rfPipeNum){
      Serial.print("Message received from node ");
      Serial.println(_sensorNodeMessage.nodeID);
      newDataFromNode = _sensorNodeMessage.nodeID;
    }
    else{  
      Serial.print("CONFIG ERROR!!! PipeNumber != nodeID: ");
      Serial.print("PipeNumber: ");
      Serial.print(rfPipeNum);
      Serial.print(" nodeID: ");
      Serial.print(_sensorNodeMessage.nodeID);
    }
  }
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


void readConfig(WsReceiverConfig &_cfg){  
  _cfg.radioNetworkAddress = 0xA0A0A0FFLL;
  _cfg.radioChannel = 101;
  strcpy(_cfg.wifiSsid, "wxIoT");
  strcpy(_cfg.wifiPass,"tXgbYPy6DzYaO-U4");
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
  _cfg.tsNodeConfigArr[1].nodeID = 6;
  _cfg.tsNodeConfigArr[1].readFrequencyMs = 60000L;

}

void initRadioRx(WsReceiverConfig _cfg){
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

void initWifi(WsReceiverConfig _cfg){
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

/*
char* deblank(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}
*/

void readSensor(WsSensorNodeMessage &_sensorNodeMessage){
  delay(1);
  localSensorMessageCnt++;
  bme.takeForcedMeasurement();

  _sensorNodeMessage.nodeID = 0;
  _sensorNodeMessage.sensorSet = cfg.sensorSet;
  _sensorNodeMessage.temperature = bme.readTemperature();
  _sensorNodeMessage.pressure = (bme.readPressure() / 100.0F) + (1.2 * (cfg.elevation/10));
  _sensorNodeMessage.humidity = bme.readHumidity();
  _sensorNodeMessage.batteryVoltage = 0;
  _sensorNodeMessage.messageCnt = localSensorMessageCnt;

  newDataFromNode = _sensorNodeMessage.nodeID;

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
