#ifndef WSN_SENSOR_DATA_CACHE_H
	#define WSN_SENSOR_DATA_CACHE_H
	
	//#include "ESP8266WiFi.h"
	#include "WS_library.h"
	#include "Arduino.h" 

	#define NUMBER_OF_NODES 5
	#define TS_TEMPERATURE_PREC 1
	#define TS_HUMIDITY_PREC 1
	#define TS_PRESSURE_PREC 0
	#define TS_BATTERY_PREC 2

	class WsnSensorDataCache{
		private:
			typedef struct{
				char cTemperature[6];
				char cHumidity[6];
				char cPressure[5];
				char cBatteryVoltage[5];
				char cMessageCnt[7];
				byte sensorSet;               // bits: messageCnt, voltage, pressure, humidity, temperature.  The least significant bit is temperature
			} WsnSensorNodeData;
    	
			WsSensorNodeData sensorCacheArr[NUMBER_OF_NODES];
			void getJsonFieldValue(char* jsonString, int8_t fieldNo, char* dst);
			void setValueByIndex(uint8_t nodeID, uint8_t idx, char* value);

		public:
			WsnSensorDataCache();
			void add(WsSensorNodeMessage &sensorNodeMessage);
			void add(uint8_t nodeID, int8_t fieldMapping[], char* jsonString);
			char* getTemperature(uint8_t nodeID);
			char* getHumidity(uint8_t nodeID);
			char* getPressure(uint8_t nodeID);
			char* getBatteryVoltage(uint8_t nodeID);
			char* getMessageCnt(uint8_t nodeID);
			char* getValueByIndex(uint8_t nodeID, uint8_t idx);
			void createThingSpeakParam(uint8_t nodeID, char* dst);
			
    };    
#endif
