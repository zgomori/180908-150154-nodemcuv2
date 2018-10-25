#include "WsnGui.h"


WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
	tftUtil = TftUtil(_tft);

	tftUtil.generateColorPalette16(this->cpIconNormal, COLOR_ICON_NORMAL, COLOR_BG_STATUSBAR);
	tftUtil.generateColorPalette16(this->cpIconError,  COLOR_ICON_ERROR, COLOR_BG_STATUSBAR);

}

void WsnGui::drawBackground(){
	tft->fillRect(0,		0,		240,	24,	COLOR_BG_STATUSBAR);
	tft->fillRect(0,		24,	240,	100,	COLOR_BG_CLOCK);	
	tft->fillRect(0,		124,	120,	98,	COLOR_BG_BLOCK1);
	tft->fillRect(120,	124,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(0,		222,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(120,	222,	120,	98,	COLOR_BG_BLOCK1);
}
