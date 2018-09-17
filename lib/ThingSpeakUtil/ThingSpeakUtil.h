#ifndef THING_SPEAK_UTIL_H
	#define  THING_SPEAK_UTIL_H

	#include "ESP8266WiFi.h"  

	class ThingSpeakUtil{
		private:
			WiFiClient client; 
			char* thingSpeakAddress;
		public:
			ThingSpeakUtil(WiFiClient client, char* thingSpeakAddress);
			void updateThingSpeak(uint8_t nodeID);
			void readThingSpeak(char* apiKey, char* channel, char* json);
	};


#endif // WS_DISPLAY_H