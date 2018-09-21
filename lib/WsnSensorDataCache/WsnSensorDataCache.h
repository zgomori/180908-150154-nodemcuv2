#ifndef WSN_SENSOR_DATA_CACHE_H
	#define WSN_SENSOR_DATA_CACHE_H
	
	//#include "ESP8266WiFi.h"
	#include "WsnCommon.h"
	#include "Arduino.h" 

	#define WSN_TEMPERATURE_DECIMAL 1
	#define WSN_HUMIDITY_DECIMAL 1
	#define WSN_PRESSURE_DECIMAL 0
	#define WSN_BATTERY_DECIMAL 2

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
    	
			WsnSensorNodeData sensorCacheArr[10];
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
			byte getSensorSet(uint8_t nodeID);
			char* getValueByIndex(uint8_t nodeID, uint8_t idx);
			void createThingSpeakParam(uint8_t nodeID, char* dst);
			void dump();
			
    };    
#endif
