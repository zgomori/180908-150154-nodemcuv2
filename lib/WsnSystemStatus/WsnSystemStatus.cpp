#include "WsnSystemStatus.h"

WsnSystemStatus::WsnSystemStatus(){
}

void WsnSystemStatus::set(statusType_enum elem, boolean value){
	if (!bitRead(this->changeBits,elem)){
		bitWrite(this->statusBits, elem, value); 
		bitWrite(this->changeBits, elem, bitRead(this->statusBits, elem) ^ value);
	}
}

boolean WsnSystemStatus::get(statusType_enum elem){
    return bitRead(this->statusBits, elem);
}

boolean WsnSystemStatus::isChanged(statusType_enum elem){
	return bitRead(this->changeBits, elem);
}

uint8_t WsnSystemStatus::getStatusBits(){
	return this->statusBits;
}

uint8_t WsnSystemStatus::getChangeBits(){
	return this->changeBits;
}

void WsnSystemStatus::resetChangeBits(){
	this->changeBits = 0;
}

