#include "ThingSpeakUtil.h"

ThingSpeakUtil::ThingSpeakUtil(WiFiClient client, char* thingSpeakAddress){
	this->client = client;
	this->thingSpeakAddress = thingSpeakAddress;
}

void ThingSpeakUtil::update(char* apiKey, char* params){
/*
	char params[70] = "\0";
	sensorDataCache.createThingSpeakParam(nodeID, params);
*/
	Serial.println(params); 

	//  Serial.print("Connecting to ");
	Serial.println(apiKey);
	if (client.connect(thingSpeakAddress, 80)) {
		Serial.println("Wifi client connected to server.");
		client.print(F("POST /update HTTP/1.1\n"));
		client.print(F("Host: api.thingspeak.com\n"));
		client.print(F("Connection: close\n"));
		client.print(F("X-THINGSPEAKAPIKEY: "));
		client.print(apiKey);
		client.print(F("\n"));
		client.print(F("Content-Type: application/x-www-form-urlencoded\n"));
		client.print(F("Content-Length: "));
		client.print(strlen(params));
		client.print(F("\n\n"));
		client.print(params);

    	Serial.println("POST request sent.");

		unsigned long clientTimeout = millis();
		while (client.available() == 0) {
			if (millis() - clientTimeout > 5000) {
				Serial.println("Client Timeout !");
				client.stop();
				return;
			}
			delay(10);		}
		/*
		Serial.println("===RESPONSE BEGIN===");
		while(client.available()){
			String line = client.readStringUntil('\r');
			Serial.println(line);
		}
		Serial.println("===RESPONSE END===");
		*/
		client.stop();
		Serial.println("===CLIENT STOP===");
  }
  else{
    Serial.println("Connection failed.");
  }


}

void ThingSpeakUtil::get(char* apiKey, char* channel, char* json){
	char getParam[80] =  "GET /channels/";
	strcat(getParam, channel);
	strcat(getParam, "/feeds/last.json?api_key=");
	strcat(getParam, apiKey);
	strcat(getParam, " HTTP/1.1\n");

	if (client.connect(thingSpeakAddress, 80)) {
		Serial.println("readThingSpeak: Wifi client connected to server.");
		Serial.println(getParam);
		client.print(getParam);
		client.print(F("Host: api.thingspeak.com\n"));
		client.print(F("Connection: close\n"));
		client.print(F("\n\n"));
 
		unsigned long clientTimeout = millis();
		while (client.available() == 0) {
			if (millis() - clientTimeout > 5000) {
				Serial.println("Client Timeout !");
				client.stop();
				return;
			}
			delay(10);    
		}
		Serial.println("===RESPONSE BEGIN===");
		char c;
		boolean startJson = false;
		uint8_t buffIdx = 0;
		while(client.available()){
			c = client.read();
			if (c == '{'){
				startJson = true;
			} 
			if (startJson){
				json[buffIdx] = c;
				buffIdx++;
			}
			if (c == '}'){
				break;
			}
		}
		json[buffIdx] = 0;

		Serial.println(json);
		client.stop();
	}
	else{
		Serial.println("Connection failed.");
	}
}