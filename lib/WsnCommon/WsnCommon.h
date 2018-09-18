#ifndef WSN_COMMON_H
#define WSN_COMMON_H

#include "Arduino.h"

#define WSN_TEMP 0
#define WSN_HUM 1
#define WSN_AIRP 2
#define WSN_BV 3
#define WSN_MC 4


typedef struct{
  uint64_t  radioNetworkAddress;        // 8 digit hex address. (0xA0A0A0FFLL)  Full tx address is radioNetworkAddress + nodeID (0xA0A0A0FFLL01)
  uint8_t   nodeID;            			// valid numbers: 1 - 5 (index of radioTxAddressArr)
  int       radioChannel;
  byte      sensorSet;                  // bits: messageCnt, voltage, pressure, humidity, temperature.  The least significant bit is temperature
  int       sleep8secCycle;             // 22 * 8s = 176s ~ 3 munute
  int       batteryMeasureCycle;        // number of loop cycles between two measurement
  float     aref1v1RaelVoltage;         // 
  float     batteryVoltageDividerRatio;  //
} WsSensorNodeConfig;

typedef struct{ 

  char      name[16];
  char      thingSpeakReadKey[20];
  char      thingSpeakChannel[8];
  int8_t    fieldMapping[5] = {-1, -1, -1, -1, -1}; // Mapping thingSpeak field numbers to sensor types. Fist element is temp, second is humidity etc. 
  uint8_t   nodeID;         // first free nodeID is 6. (0 is the local sensor, 1-5 are the radio nodes)
  uint64_t  readCycleMs;
} WsTSnodeConfig;

typedef struct{
  uint64_t  radioNetworkAddress;        // 8 digit hex address. (0xA0A0A0FFLL)  Full tx address is radioNetworkAddress + nodeID (0xA0A0A0FFLL01)
  int       radioChannel;
  char 		wifiSsid[20];
  char 		wifiPass[20];
  char      thingSpeakAddress[20] = "api.thingspeak.com";
  char		thingSpeakAPIKeyArr[5][17];
  byte      sensorSet;                  // bits: messageCnt, voltage, pressure, humidity, temperature.  The least significant bit is temperature
  uint64_t  sensorReadCycleMs;          // sensor reading ms
  float		elevation;					// elevation for calculation of relative air pressure (sea level pressure);
  WsTSnodeConfig tsNodeConfigArr[2];
} WsReceiverConfig;

typedef struct{
  const byte messageType = 1;   // For furthure development 
  uint8_t nodeID;				// valid numbers: 1 - 5; value from WsSensorNodeConfig
  byte sensorSet;               // bits: messageCnt, voltage, pressure, humidity, temperature.  The least significant bit is temperature
  byte udef = 0;
  float temperature;
  float humidity;
  float pressure;
  float batteryVoltage;
  uint32_t messageCnt;
} WsSensorNodeMessage;

typedef struct{
  byte sensorSet;               // bits: messageCnt, voltage, pressure, humidity, temperature.  The least significant bit is temperature
  char cTemperature[6];
  char cHumidity[6];
  char cPressure[5];
  char cBatteryVoltage[5];
  char cMessageCnt[7];
} WsSensorNodeData;

#endif