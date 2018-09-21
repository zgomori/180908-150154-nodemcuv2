#include "WsnTimer.h"

WsnTimer::WsnTimer(){
}

void WsnTimer::init(WsnReceiverConfig &cfg){
	timerArr[0].nodeID = 0;
	timerArr[0].frequencyMs = cfg.sensorReadFrequencyMs;
	timerArr[0].lastFireMillis = 0;
	uint8_t tsNodes = sizeof(cfg.tsNodeConfigArr) / sizeof(cfg.tsNodeConfigArr[0]); 
	for(uint8_t i = 0; i < tsNodes; i++){ 
		timerArr[i+1].nodeID = cfg.tsNodeConfigArr[i].nodeID;
		timerArr[i+1].tsNodeConfigIdx = i;
		timerArr[i+1].frequencyMs = cfg.tsNodeConfigArr[i].readFrequencyMs;
		timerArr[i+1].lastFireMillis = 0;
	}
}

void WsnTimer::setTriggerFunction(TriggerFunction tf){
	triggerFunction = tf;
}	

void WsnTimer::ticker(){
	uint32_t ms = millis();
	for(uint8_t i = 0; i < 5; i++ ){
		if (timerArr[i].nodeID == -1){
			break;
		}
		if(timerArr[i].lastFireMillis + timerArr[i].frequencyMs <= ms){
			timerArr[i].lastFireMillis = ms;
			yield();
			triggerFunction(timerArr[i].nodeID, timerArr[i].tsNodeConfigIdx);
			yield();	
			break;
		} 
	}		
}

