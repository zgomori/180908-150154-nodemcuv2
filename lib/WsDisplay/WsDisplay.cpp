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
	displayBlockArr[blockArrIdx].labelChain = NULL;

	return blockArrIdx;
}

uint8_t WsDisplay::defineField(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t  _h, uint16_t _fgColor, const GFXfont *_font, align_enum _align, char *_dataPtr, int8_t _blockIdx){
	defineTextElement(_x, _y, _w, _h,  _fgColor, _font, _align, _dataPtr, _blockIdx, 'F');
	if(!displayBlockArr[_blockIdx].fieldChain){
		displayBlockArr[_blockIdx].fieldChain = &textElementArr[textElementArrIdx];
	}
	else{
		for(int8_t i = textElementArrIdx-1; i > -1; i--){
			if((textElementArr[i].blockIdx == _blockIdx) && (textElementArr[i].type == 'F') && (!textElementArr[i].fieldChain)){
				textElementArr[i].fieldChain = &textElementArr[textElementArrIdx];
				break;
			}
		}
	}
	return textElementArrIdx;
}

uint8_t WsDisplay::defineLabel(uint8_t _x, uint16_t _y, uint16_t _fgColor, const GFXfont *_font,  char *_dataPtr, int8_t _blockIdx){
	defineTextElement(_x, _y, 0, 0,  _fgColor, _font, LEFT, _dataPtr, _blockIdx, 'L');
	if(!displayBlockArr[_blockIdx].labelChain){
		displayBlockArr[_blockIdx].labelChain = &textElementArr[textElementArrIdx];
	}
	else{
		for(int8_t i = textElementArrIdx-1; i > -1; i--){
			if((textElementArr[i].blockIdx == _blockIdx) && (textElementArr[i].type == 'L') && (!textElementArr[i].labelChain)){
				textElementArr[i].labelChain = &textElementArr[textElementArrIdx];
				break;
			}
		}
	}
	return textElementArrIdx;
}

void WsDisplay::defineTextElement(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t  _h, uint16_t _fgColor, const GFXfont *_font, align_enum _align, char *_dataPtr, int8_t _blockIdx, char _type){
	textElementArrIdx++;
	textElementArr[textElementArrIdx].x = displayBlockArr[_blockIdx].x + _x;
	textElementArr[textElementArrIdx].y = displayBlockArr[_blockIdx].y + _y;
	textElementArr[textElementArrIdx].w = _w;
	textElementArr[textElementArrIdx].h = _h;
	textElementArr[textElementArrIdx].fgColor = _fgColor;
	textElementArr[textElementArrIdx].font = _font;
	textElementArr[textElementArrIdx].align = _align;
	textElementArr[textElementArrIdx].dataPtr = _dataPtr;
	textElementArr[textElementArrIdx].blockIdx = _blockIdx;
	textElementArr[textElementArrIdx].type = _type;
}


void WsDisplay::showField(int8_t fieldIdx, bool fill){
	showField(&textElementArr[fieldIdx], fill);
}

void WsDisplay::showField(TextElement* textElement, bool fill){
	DisplayBlock* wBlock = &displayBlockArr[textElement->blockIdx];
	int cursorX;
	int cursorY;
	int16_t  tempX, tempY;
	uint16_t tempW, tempH;

	_tft->setFont(textElement->font);

	cursorY = textElement->y + textElement->h;
	if(textElement->align == LEFT){
		cursorX = textElement->x;
	}
	else{
		_tft->getTextBounds(textElement->dataPtr, 0, 0, &tempX, &tempY, &tempW, &tempH); 
		cursorX = textElement->x + (textElement->w - tempW)/2; 
	}
	if (fill){
		_tft->fillRect(textElement->x, textElement->y, textElement->w , textElement->h, wBlock->bgColor);
	}

	_tft->setCursor(cursorX, cursorY);
	_tft->print(textElement->dataPtr);
	//_tft->drawFastHLine(cursorX, cursorY, tempW, ILI9341_WHITE);
	yield();
}

void WsDisplay::showLabel(TextElement* textElement){
	_tft->setFont(textElement->font);
	_tft->setCursor(textElement->x, textElement->y);
	_tft->print(textElement->dataPtr);
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

		TextElement* labelPtr = wBlock->labelChain;
		while(labelPtr){
			showLabel(labelPtr);
			labelPtr = (TextElement*)labelPtr->labelChain;
			yield();
		}	

	}
	TextElement* fieldPtr = wBlock->fieldChain;
	while(fieldPtr){
		showField(fieldPtr,!fill);
		fieldPtr = (TextElement*)fieldPtr->fieldChain;
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