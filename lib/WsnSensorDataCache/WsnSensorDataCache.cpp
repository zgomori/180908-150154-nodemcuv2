#include "WsnSensorDataCache.h"

    WsnSensorDataCache::WsnSensorDataCache(){
    }

    void WsnSensorDataCache::add(WsSensorNodeMessage &sensorNodeMessage){
      char charConvBuffer[10];
      sensorCacheArr[sensorNodeMessage.nodeID].sensorSet = sensorNodeMessage.sensorSet;
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cTemperature, dtostrf(sensorNodeMessage.temperature,1,WSN_TEMPERATURE_PREC,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cHumidity, dtostrf(sensorNodeMessage.humidity,1,WSN_HUMIDITY_PREC,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cPressure, dtostrf(sensorNodeMessage.pressure,1,WSN_PRESSURE_PREC,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cBatteryVoltage, dtostrf(sensorNodeMessage.batteryVoltage,1,WSN_BATTERY_PREC,charConvBuffer));
      strcpy(sensorCacheArr[sensorNodeMessage.nodeID].cMessageCnt, dtostrf(sensorNodeMessage.messageCnt,1,0,charConvBuffer));
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

