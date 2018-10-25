#include "WsnGui.h"

// remove!!!
	
	#define COLOR1 0x18C3
	#define COLOR2 0x10A2

WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
}

void WsnGui::drawBackground(){
	tft->fillRect(0,		0,		240,	24,	COLOR1);
	tft->fillRect(0,		24,	240,	100,	TFT_BLACK);	
	tft->fillRect(0,		124,	120,	98,	COLOR1);
	tft->fillRect(120,	124,	120,	98,	COLOR2);
	tft->fillRect(0,		222,	120,	98,	COLOR2);
	tft->fillRect(120,	222,	120,	98,	COLOR1);
}
