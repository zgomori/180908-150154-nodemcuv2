#include "WsnGui.h"


WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
	tftUtil = TftUtil(_tft);

	tftUtil.generateColorPalette16(COLOR_ICON_NORMAL, COLOR_BG_STATUSBAR, cpIconNormal);
	tftUtil.generateColorPalette16(COLOR_ICON_ERROR, COLOR_BG_STATUSBAR, cpIconError);

}

void WsnGui::drawBackground(){
  tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK1);
  tft->setTextDatum(ML_DATUM);


	tft->fillRect(0,		0,		240,	24,	COLOR_BG_STATUSBAR);
	tft->fillRect(0,		24,	240,	100,	COLOR_BG_CLOCK);	

	tft->fillRect(0,		124,	120,	98,	COLOR_BG_BLOCK1);


	tft->fillRect(120,	124,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(0,		222,	120,	98,	COLOR_BG_BLOCK2);
	tft->fillRect(120,	222,	120,	98,	COLOR_BG_BLOCK1);

  tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK1);
  tft->setTextDatum(ML_DATUM);
  tft.drawString("livingroom", bx+4, by+10, 2);

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

void WsnGui::displaySensorData(const uint8_t sensorID, const WsnSensorDataCache &sensorDataCache){
tft.setTextColor(TFT_DARKGREY,COLOR1);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("livingroom", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR1);
  tft.setFreeFont(CF_OL32);
  tft.drawString("20.1", bx+60, by+35, 1);
 // tft.setFreeFont(CF_OL24);
  tft.setFreeFont(CF_ORB);

  tft.drawString("37.0", bx+60, by+70, 1);

/*B2********************************************************/  
  bx = b2x;
  by = b2y;

  tft.setTextColor(TFT_DARKGREY,COLOR2);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("child's room", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR2);  
  tft.setFreeFont(CF_OL32);
  tft.drawString("21.1", bx+60, by+35, 1);
  //tft.setFreeFont(CF_OL24);
  tft.setFreeFont(CF_ORB);

  tft.drawString("56.8", bx+60, by+70, 1);

/*B3********************************************************/  
  bx = b3x;
  by = b3y;

  tft.setTextColor(TFT_DARKGREY,COLOR2);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("outdoor", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR2);
  tft.setFreeFont(CF_OL32);
  tft.drawString("-5.5", bx+60, by+35, 1);
  tft.setFreeFont(CF_OL24);
  tft.drawString("13.8", bx+60, by+70, 1);

/*B4********************************************************/ 
  bx = b4x;
  by = b4y;

  tft.setTextColor(TFT_DARKGREY,COLOR1);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("atm.pressure", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN,COLOR1);
  tft.setFreeFont(CF_OL24);
  tft.drawString("1015", bx+60, by+34, 1);

}