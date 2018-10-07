#ifndef NTP_CLIENT_H
	#define NTP_CLIENT_H

	#include "Arduino.h"
	#include "ESP8266WiFi.h"
	#include "WiFiUdp.h"
	#include "TimeLib.h"
	
	#define NTP_PACKET_SIZE 48

	class NTPclient{
		private:
			WiFiUDP 	udp;
			char*		ntpServerName;
			int8_t  	timeZone;
			int8_t	dstCrrectedTimeZone;
			uint	   localUdpPort; 
			byte 		packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

			void sendNTPpacket(IPAddress &address);
			bool isDst(time_t theTime);

		public:
			NTPclient();
			void init(WiFiUDP &_udp, char* _ntpServerName, uint _localUdpPort, int8_t _timeZone);
			time_t getTime();
	};	

#endif
