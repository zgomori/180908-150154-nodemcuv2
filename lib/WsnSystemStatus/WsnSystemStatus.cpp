#include "WsnSystemStatus.h"


Boolean_t::Boolean_t(){
	value = -1;
}

Boolean_t::Boolean_t(bool newValue){
	value = (int8_t)newValue;
}

void Boolean_t::setValue(bool newValue){
	value = (int8_t)newValue;
}

void Boolean_t::setValue(boolean_enum newValue){
	value = newValue;
}

int8_t Boolean_t::getValue(){
	return value;
}

bool Boolean_t::isTrue(){
	return (value == this->TRUE_);
}

bool Boolean_t::isFalse(){
	return (value == this->FALSE_);
}

bool Boolean_t::isNull(){
	return (value == this->NULL_);
}

bool Boolean_t::isNotNull(){
	return (value != this->NULL_);
}

WsnSystemStatus::WsnSystemStatus(){
}

void WsnSystemStatus::set(statusType_enum elem, bool value){
	if (!bitRead(changeBits,elem)){
		if(statusArr[elem].getValue() != (int8_t)value){
			statusArr[elem].setValue(value);
			bitSet(changeBits, elem);
		}
	}
}

Boolean_t WsnSystemStatus::get(statusType_enum elem){
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

