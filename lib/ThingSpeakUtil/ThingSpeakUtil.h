#ifndef THING_SPEAK_UTIL_H
	#define  THING_SPEAK_UTIL_H

	#include "ESP8266WiFi.h"  

	class ThingSpeakUtil{
		private:
			WiFiClient *client; 
			char* thingSpeakAddress;
		public:
			ThingSpeakUtil(WiFiClient *wifiClient, char* thingSpeakAddress);
			bool update(char* apiKey, char* params);
			bool get(char* apiKey, char* channel, char* json);
	};

#endif // WS_DISPLAY_H