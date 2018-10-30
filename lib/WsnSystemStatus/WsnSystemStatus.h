#ifndef WSN_SYSTEM_STATUS_H
	#define WSN_SYSTEM_STATUS_H
	
	#include "Arduino.h" 

	class WsnSystemStatus{
		private:
			int8_t statusArr[6] = {-1, -1, -1, -1, -1, -1};
			uint8_t changeBits = 0; 
		public:
			enum statusType_enum {
				WIFI = 0,
				RADIO = 1,
				LOCAL_SENSOR = 2,
				NTP = 3,
				TS_UPDATE = 4,
				TS_GET = 5
			};

			WsnSystemStatus();

			void set(statusType_enum elem, bool value);
			bool get(statusType_enum elem);
			bool isChanged(statusType_enum elem);
			uint8_t getChangeBits();
			void resetChangeBits(); 
	};    
#endif