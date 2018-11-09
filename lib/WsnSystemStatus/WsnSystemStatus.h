#ifndef WSN_SYSTEM_STATUS_H
	#define WSN_SYSTEM_STATUS_H
	
	#include "Arduino.h" 

	class Boolean_t{
		private:
			int8_t value = -1;

		public:
			enum boolean_enum{
				NULL_ = -1,
				FALSE_ = 0,
				TRUE_ = 1
			};

			Boolean_t();
			Boolean_t(bool newValue);
			void setValue(bool newValue);
			void setValue(boolean_enum newValue);
			int8_t getValue();
			bool isTrue();
			bool isFalse();
			bool isNull();
			bool isNotNull();
	};

	class WsnSystemStatus{
		private:
			Boolean_t statusArr[6];
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
			Boolean_t get(statusType_enum elem);
			bool isChanged(statusType_enum elem);
			uint8_t getChangeBits();
			void resetChangeBits(); 
	};    
#endif