#include "WsnGui.h"

// remove!!!
	
	#define COLOR1 0x18C3
	#define COLOR2 0x10A2

WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
}

void WsnGui::drawBackground(){
	uint8_t bx;
	uint8_t by;

	uint8_t bw = 120;
	uint8_t bh = 98;

	uint8_t b1x = 0;
	uint8_t b1y = 124;

	uint8_t b2x = b1x+bw;
	uint8_t b2y = b1y;

	uint8_t b3x = b1x;
	uint8_t b3y = b1y+bh;

	uint8_t b4x = b1x+bw;
	uint8_t b4y = b1y+bh;  	  

	tft->fillRect(0,  0, 240, 24, COLOR1);
	tft->fillRect(b1x,  b1y, bw, bh, COLOR1);
	tft->fillRect(b2x,  b2y, bw, bh, COLOR2);
	tft->fillRect(b3x,  b3y, bw, bh, COLOR2);
	tft->fillRect(b4x,  b4y, bw, bh, COLOR1);
}
