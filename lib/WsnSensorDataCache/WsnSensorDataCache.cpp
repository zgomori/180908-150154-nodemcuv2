#include "WsnSensorDataCache.h"

    WsnSensorDataCache::WsnSensorDataCache(){
      for(int8_t i = 0; i < sizeof(sensorCacheArr)/sizeof(sensorCacheArr[0]);i++){
      sensorCacheArr[i].sensorSet = 0;
      strcpy(sensorCacheArr[i].cTemperature, "0.0");
      strcpy(sensorCacheArr[i].cHumidity, "0.0");
      strcpy(sensorCacheArr[i].cPressure, "0");
      strcpy(sensorCacheArr[i].cBatteryVoltage, "0.00");
      strcpy(sensorCacheArr[i].cMessageCnt, "0");
		sensorCacheArr[i].lastUpdateTime = 0;
      }
    }

    void WsnSensorDataCache::add(WsnSensorNodeMessage &sensorNodeMessage){
      char charConvBuffer[10];
      sensorCacheArr[sensorNodeMessage.nodeID].sensorSet = sensorNodeMessage.sensorSet;
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cTemperature, dtostrf(sensorNodeMessage.temperature,1,WSN_TEMPERATURE_DECIMAL,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cHumidity, dtostrf(sensorNodeMessage.humidity,1,WSN_HUMIDITY_DECIMAL,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cPressure, dtostrf(sensorNodeMessage.pressure,1,WSN_PRESSURE_DECIMAL,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cBatteryVoltage, dtostrf(sensorNodeMessage.batteryVoltage,1,WSN_BATTERY_DECIMAL,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cMessageCnt, dtostrf(sensorNodeMessage.messageCnt,1,0,charConvBuffer));
		sensorCacheArr[sensorNodeMessage.nodeID].lastUpdateTime = now();
    }

    void WsnSensorDataCache::add(uint8_t nodeID, int8_t fieldMapping[], char* jsonString){
      char jsonValue[10];
      sensorCacheArr[nodeID].sensorSet = 0;
      for(uint8_t i=0; i < 5; i++){
        int8_t fieldNo = fieldMapping[i];
        if (fieldNo != -1){
            getJsonFieldValue(jsonString, fieldNo,  jsonValue);
            setValueByIndex(nodeID, i, jsonValue);
            bitSet(sensorCacheArr[nodeID].sensorSet,i);
        }
      }
		sensorCacheArr[nodeID].lastUpdateTime = now();
    }  
    
    char* WsnSensorDataCache::getTemperature(uint8_t nodeID){
      return sensorCacheArr[nodeID].cTemperature;
    }

    char* WsnSensorDataCache::getHumidity(uint8_t nodeID){
      return sensorCacheArr[nodeID].cHumidity;
    }

    char* WsnSensorDataCache::getPressure(uint8_t nodeID){
      return sensorCacheArr[nodeID].cPressure;
    }

    char* WsnSensorDataCache::getBatteryVoltage(uint8_t nodeID){
      return sensorCacheArr[nodeID].cBatteryVoltage;
    }

    char* WsnSensorDataCache::getMessageCnt(uint8_t nodeID){
      return sensorCacheArr[nodeID].cMessageCnt;
    }

    byte WsnSensorDataCache::getSensorSet(uint8_t nodeID){
       return sensorCacheArr[nodeID].sensorSet; 
    }

    time_t WsnSensorDataCache::getLastUpdateTime(uint8_t nodeID){
       return sensorCacheArr[nodeID].lastUpdateTime; 
    }

	 void WsnSensorDataCache::getLastUpdateTimeF(uint8_t nodeID, char *timeString){
		 	sprintf(timeString, "%02u:%02u:%02u", hour(sensorCacheArr[nodeID].lastUpdateTime), minute(sensorCacheArr[nodeID].lastUpdateTime), second(sensorCacheArr[nodeID].lastUpdateTime));
	 }

    char* WsnSensorDataCache::getValueByIndex(uint8_t nodeID, uint8_t idx){
      static uint8_t ptrOffset[5] = {0,6,12,17,22};
      char* ptr = sensorCacheArr[nodeID].cTemperature;
      return ptr + ptrOffset[idx];
    }

    void WsnSensorDataCache::setValueByIndex(uint8_t nodeID, uint8_t idx, char* value){
      static uint8_t ptrOffset[5] = {0,6,12,17,22};
      char* ptr = sensorCacheArr[nodeID].cTemperature;
      strcpy(ptr + ptrOffset[idx],value);
    }


    void WsnSensorDataCache::createThingSpeakParam(uint8_t nodeID, char* dst){
      static const char urlParamConstArray[13] = "field12345=&";
      uint8_t fieldCnt = 0;
      
      for(uint8_t i=0; i<5; i++){
        if (bitRead(sensorCacheArr[nodeID].sensorSet,i)){
          fieldCnt++;
          strncat(dst, &urlParamConstArray[0],5);
          strncat(dst, &urlParamConstArray[fieldCnt+4],1);
          strncat(dst, &urlParamConstArray[10],1);
          strcat(dst, getValueByIndex(nodeID,i));
          strncat(dst, &urlParamConstArray[11],1);
        }
      }      
    }

    void WsnSensorDataCache::getJsonFieldValue(char* jsonString, int8_t fieldNo, char* dst){
      char key[]="\"fieldX\"";
      key[6] = '0' + fieldNo;
      char* src = strstr(jsonString+45, key);
      dst[0] = 0;
      if(src){
        //  "fieldX":"
        //            ^ position src + 10
        src = src + 10;
        uint8_t i = 0;
        while( (src[i] != 0) && (src[i] != '\"') && (i < 9)) {
          dst[i] = src[i];
          i++;  
        }
        dst[i] = 0;
      }
  }

	void WsnSensorDataCache::getDumpRow(int8_t nodeID, char *buff){
		char timeBuff[9];
		getLastUpdateTimeF(nodeID, timeBuff);
		sprintf(buff,   "%2u%7s%8s%8s%8s%9s%11s", nodeID, getTemperature(nodeID), getHumidity(nodeID), getPressure(nodeID), getBatteryVoltage(nodeID), getMessageCnt(nodeID), timeBuff);
		yield();
	}

	void WsnSensorDataCache::printData(int8_t nodeID){
		char buff[64];
		Serial.println(dumpTitle);
		Serial.println(dumpHeader);
		getDumpRow(nodeID, buff);
		Serial.println(buff);
		Serial.println(dumpFooter);
	}

	void WsnSensorDataCache::dump(){
		char buff[64];
		Serial.println(dumpTitle);
		Serial.println(dumpHeader);
		for(int8_t i = 0; i < sizeof(sensorCacheArr)/sizeof(sensorCacheArr[0]);i++){
			getDumpRow(i, buff);
			Serial.println(buff);
		}	
		Serial.println(dumpFooter);
	}

	uint8_t WsnSensorDataCache::size(){
		return sizeof(sensorCacheArr)/sizeof(sensorCacheArr[0]);
	}