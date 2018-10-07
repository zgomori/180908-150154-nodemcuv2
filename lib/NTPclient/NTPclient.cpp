#include "NTPclient.h"

NTPclient::NTPclient(){
}

void NTPclient::init(WiFiUDP &_udp, char* _ntpServerName, uint _localUdpPort, int8_t _timeZone){
	udp = _udp;
	ntpServerName = _ntpServerName;
	localUdpPort = _localUdpPort;
	timeZone = _timeZone;
	dstCrrectedTimeZone = _timeZone;

	
	udp.begin(localUdpPort);
}

time_t NTPclient::getTime(){
	IPAddress ntpServerIP; // NTP server's ip address

	while (udp.parsePacket() > 0) ; // discard any previously received packets
	Serial.println("Transmit NTP Request");
	// get a random server from the pool
	WiFi.hostByName(ntpServerName, ntpServerIP);
	Serial.print(ntpServerName);
	Serial.print(": ");
	Serial.println(ntpServerIP);
	sendNTPpacket(ntpServerIP);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];

			//!!!!!!
			unsigned long ret = secsSince1900 - 2208988800UL + dstCrrectedTimeZone * SECS_PER_HOUR;
			if(isDst(ret)){
				dstCrrectedTimeZone = timeZone + 1;
				ret = secsSince1900 - 2208988800UL + dstCrrectedTimeZone * SECS_PER_HOUR;
				Serial.println("set Daylight Saving Time");
			}
			//!!!!!!
			return ret;
		}
	}
	Serial.println("[ERROR] UDPclient.NgetTime: no NTP Response");
	return 0; // return 0 if unable to get the time
}

void NTPclient::sendNTPpacket(IPAddress &address){
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}

bool NTPclient::isDst(time_t theTime){
	int _month = month(theTime);
	int _day = day(theTime);
	int _dow = weekday(theTime);

	if (_month < 3 || _month > 10)  return false; 
	if (_month > 3 && _month < 10)  return true; 

	int previousSunday = _day - _dow;

	if (_month == 3) return previousSunday >= 25;
	if (_month == 10) return previousSunday < 25;
	return false; // this line never gonna happend
}