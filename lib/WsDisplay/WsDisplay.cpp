#include "WsDisplay.h"

WsDisplay::WsDisplay(Adafruit_ILI9341 *tft){
	this->_tft = tft;
}

uint8_t WsDisplay::defineBlock(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t _h, uint16_t _bgColor, int8_t _screenIdx){
	blockArrIdx++;
	displayBlockArr[blockArrIdx].x = _x;
	displayBlockArr[blockArrIdx].y = _y;
	displayBlockArr[blockArrIdx].w = _w;
	displayBlockArr[blockArrIdx].h = _h;
	displayBlockArr[blockArrIdx].bgColor = _bgColor;
	displayBlockArr[blockArrIdx].screenIdx = _screenIdx;
	displayBlockArr[blockArrIdx].fieldChain = NULL;

	return blockArrIdx;
}

uint8_t WsDisplay::defineDataField(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t  _h, uint16_t _fgColor, const GFXfont *_font, align_enum _align, char *_dataPtr, int8_t _blockIdx){
	dataFieldArrIdx++;
	dataFieldArr[dataFieldArrIdx].x = displayBlockArr[_blockIdx].x + _x;
	dataFieldArr[dataFieldArrIdx].y = displayBlockArr[_blockIdx].y + _y;
	dataFieldArr[dataFieldArrIdx].w = _w;
	dataFieldArr[dataFieldArrIdx].h = _h;
	dataFieldArr[dataFieldArrIdx].fgColor = _fgColor;
	dataFieldArr[dataFieldArrIdx].font = _font;
	dataFieldArr[dataFieldArrIdx].align = _align;
	dataFieldArr[dataFieldArrIdx].dataPtr = _dataPtr;
	dataFieldArr[dataFieldArrIdx].blockIdx = _blockIdx;

	if(!displayBlockArr[_blockIdx].fieldChain){
		displayBlockArr[_blockIdx].fieldChain = &dataFieldArr[dataFieldArrIdx];
	}
	else{
		for(int8_t i = dataFieldArrIdx-1; i > -1; i--){
			if((dataFieldArr[i].blockIdx == _blockIdx) && (!dataFieldArr[i].fieldChain)){
				dataFieldArr[i].fieldChain = &dataFieldArr[dataFieldArrIdx];
				break;
			}
		}
	}
	return dataFieldArrIdx;
}


void WsDisplay::showData(int8_t fieldIdx, bool fill){
	showData(&dataFieldArr[fieldIdx], fill);
}

void WsDisplay::showData(DataField* dataField, bool fill){
	DisplayBlock* wBlock = &displayBlockArr[dataField->blockIdx];
	int cursorX;
	int cursorY;
	int16_t  tempX, tempY;
	uint16_t tempW, tempH;

	_tft->setFont(dataField->font);

	cursorY = dataField->y + dataField->h;
	if(dataField->align == LEFT){
		cursorX = dataField->x;
	}
	else{
		_tft->getTextBounds(dataField->dataPtr, 0, 0, &tempX, &tempY, &tempW, &tempH); 
		cursorX = dataField->x + (dataField->w - tempW)/2; 
	}
	if (fill){
		_tft->fillRect(dataField->x, dataField->y, dataField->w , dataField->h, wBlock->bgColor);
	}

	_tft->setCursor(cursorX, cursorY);
	_tft->print(dataField->dataPtr);
	//_tft->drawFastHLine(cursorX, cursorY, tempW, ILI9341_WHITE);
	yield();
}

void WsDisplay::showBlock(int8_t blockIdx){
	return showBlock(blockIdx, false);
}

void WsDisplay::showBlock(int8_t blockIdx, bool fill){
	DisplayBlock* wBlock = &displayBlockArr[blockIdx];
	if(fill){
		_tft->fillRect(wBlock->x,wBlock->y, wBlock->w, wBlock->h, wBlock->bgColor);
		yield();
	}
	DataField* fieldPtr = wBlock->fieldChain;
	while(fieldPtr){
		showData(fieldPtr,!fill);
		fieldPtr = (DataField*)fieldPtr->fieldChain;
		yield();
	}	
}

void WsDisplay::showScreen(){
	int8_t blockArrSize = sizeof(displayBlockArr)/sizeof(DisplayBlock);
	for(int8_t i=0; i < blockArrSize; i++){
		if(displayBlockArr[i].screenIdx == currentScreenIdx){
			showBlock(i,true);
		}
	}
}

void WsDisplay::switchScreen(int8_t screenIdx){
	currentScreenIdx = screenIdx;
	showScreen();
}

int8_t WsDisplay::getCurrentScreen(){
	return currentScreenIdx;
}