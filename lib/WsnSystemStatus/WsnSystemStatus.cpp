#include "WsnSystemStatus.h"

WsnSystemStatus::WsnSystemStatus(){
}

void WsnSystemStatus::set(statusType_enum elem, bool value){
	if (!bitRead(changeBits,elem)){
		if(statusArr[elem] != (uint8_t)value){
			statusArr[elem] = (uint8_t)value;
			bitSet(changeBits, elem);
		}
	}
}

bool WsnSystemStatus::get(statusType_enum elem){
	return (bool)statusArr[elem];
}

int8_t WsnSystemStatus::getInt(statusType_enum elem){
	return statusArr[elem];
}

bool WsnSystemStatus::isChanged(statusType_enum elem){
	return bitRead(changeBits, elem);
}

uint8_t WsnSystemStatus::getChangeBits(){
	return changeBits;
}

void WsnSystemStatus::resetChangeBits(){
	changeBits = 0;
}

