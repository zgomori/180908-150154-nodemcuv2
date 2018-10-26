#include "WsnGui.h"


WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
	tftUtil = TftUtil(_tft);

	tftUtil.generateColorPalette16(cpIconNormal, COLOR_ICON_NORMAL, COLOR_BG_STATUSBAR);
	tftUtil.generateColorPalette16(cpIconError,  COLOR_ICON_ERROR, COLOR_BG_STATUSBAR);

}

void WsnGui::drawBackground(){
	tft->fillRect(0,		0,		240,	24,	COLOR_BG_STATUSBAR);
	tft->fillRect(0,		24,	240,	100,	COLOR_BG_CLOCK);	
	tft->fillRect(0,		124,	120,	98,	COLOR_BG_BLOCK1);
	tft->fillRect(120,	124,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(0,		222,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(120,	222,	120,	98,	COLOR_BG_BLOCK1);
}

void WsnGui::drawWifiStatus(bool status){
	tftUtil.drawIcon4(&iconWifi, status ? cpIconNormal : cpIconError, 4, 3);
}

void WsnGui::drawRadioStatus(bool status){
	tftUtil.drawIcon4(&iconRadio, status ? cpIconNormal : cpIconError, 34, 2);
}

void WsnGui::drawSensorStatus(bool status){
	tftUtil.drawIcon4(&iconSensor, status ? cpIconNormal : cpIconError, 62, 2);
}


void WsnGui::refreshStatusBar(WsnSystemStatus &sysStat){
	if (sysStat.getChangeBits() != 0){
		if (sysStat.isChanged(sysStat.WIFI)){
			drawWifiStatus(sysStat.get(sysStat.WIFI));
		}
		if (sysStat.isChanged(sysStat.RADIO)){
			drawRadioStatus(sysStat.get(sysStat.RADIO));
		}
		if (sysStat.isChanged(sysStat.LOCAL_SENSOR)){
			drawSensorStatus(sysStat.get(sysStat.LOCAL_SENSOR));
		}		
	}

	sysStat.resetChangeBits();
}