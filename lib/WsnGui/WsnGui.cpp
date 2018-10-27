#include "WsnGui.h"


WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
	tftUtil = TftUtil(_tft);

	tftUtil.generateColorPalette16(COLOR_ICON_NORMAL, COLOR_BG_STATUSBAR, cpIconNormal);
	tftUtil.generateColorPalette16(COLOR_ICON_ERROR, COLOR_BG_STATUSBAR, cpIconError);

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

void WsnGui::drawNtpStatus(bool status){
	tftUtil.drawIcon4(&iconClock, status ? cpIconNormal : cpIconError, 80, 4);
}

void WsnGui::drawThingSpeakUpdateStatus(bool status){
	tftUtil.drawIcon4(&iconCloudUp, status ? cpIconNormal : cpIconError, 108, 5);
}

void WsnGui::drawThingSpeakGetStatus(bool status){
	tftUtil.drawIcon4(&iconCloudDown, status ? cpIconNormal : cpIconError, 136, 5);
}

void WsnGui::updateStatusBar(WsnSystemStatus &sysStat){
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
		if (sysStat.isChanged(sysStat.NTP)){
			drawNtpStatus(sysStat.get(sysStat.NTP));
		}		
		if (sysStat.isChanged(sysStat.TS_UPDATE)){
			drawThingSpeakUpdateStatus(sysStat.get(sysStat.TS_UPDATE));
		}
		if (sysStat.isChanged(sysStat.TS_GET)){
			drawThingSpeakGetStatus(sysStat.get(sysStat.TS_GET));
		}
	}

	sysStat.resetChangeBits();
}