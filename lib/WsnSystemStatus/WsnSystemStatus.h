#ifndef WSN_SYSTEM_STATUS_H
	#define WSN_SYSTEM_STATUS_H
	
	#include "Arduino.h" 

	class WsnSystemStatus{
		private:
            uint8_t statusBits = 0;
            uint8_t changeBits = 0; 
        public:
   			enum statusType_enum {
                WIFI = 0,
                RADIO = 1,
                NTP = 2,
                LOCAL_SENSOR = 3,
                TS_UPDATE = 4,
                TS_GET = 5
            };

			WsnSystemStatus();

            void set(statusType_enum elem, boolean value);
            boolean get(statusType_enum elem);
            boolean isChanged(statusType_enum elem);
            uint8_t getStatusBits();
            uint8_t getChangeBits();
            void resetChangeBits(); 
    };    
#endif