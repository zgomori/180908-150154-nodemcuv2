#ifndef WSN_TIMER_H
	#define WSN_TIMER_H
	
	#include "Arduino.h" 
	#include "WsnCommon.h"

	class WsnTimer{
		private:
			typedef struct{
				int8_t nodeID = -1;
				int8_t tsNodeConfigIdx = -1;
				uint32_t frequencyMs = 0;
				uint32_t lastFireMillis = 0;
			} WsnTimerEntry;
    	
		 	typedef void(*TriggerFunction)(int8_t nodeID, int8_t tsNodeConfigIdx);

			WsnTimerEntry timerArr[5];
			TriggerFunction triggerFunction;

		public:
			WsnTimer();
			void init(WsnReceiverConfig &cfg);
			void setTriggerFunction(TriggerFunction tf);
			void ticker();
	};		
#endif

