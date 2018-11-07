#include "WsnGui.h"


WsnGui::WsnGui(TFT_eSPI *_tft){
	this->tft = _tft;
	tftUtil = TftUtil(_tft);

	tftUtil.generateColorPalette16(COLOR_ICON_NORMAL, COLOR_BG_STATUSBAR, cpIconNormal);
	tftUtil.generateColorPalette16(COLOR_ICON_ERROR, COLOR_BG_STATUSBAR, cpIconError);

}

uint8_t WsnGui::getCurrentScreenId(){
	return currentScreenId;
}

void WsnGui::switchScreen(uint8_t screenId){
	currentScreenId = screenId;
	switch (screenId){
		case SCR_MAIN:
			initMainScreen();
			redrawStatusBar();
			displayClock();
			displayDate();
			displaySensorData(0);
			displaySensorData(1);
			displaySensorData(6);

			break;

		case SCR_MENU:
			displaySensorDump();
			break;
	} 
	
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

void WsnGui::updateStatusBar(bool resetStatus){
	if (sysStatus.getChangeBits() != 0){
		if (sysStatus.isChanged(sysStatus.WIFI)){
			drawWifiStatus(sysStatus.get(sysStatus.WIFI));
		}
		if (sysStatus.isChanged(sysStatus.RADIO)){
			drawRadioStatus(sysStatus.get(sysStatus.RADIO));
		}
		if (sysStatus.isChanged(sysStatus.LOCAL_SENSOR)){
			drawSensorStatus(sysStatus.get(sysStatus.LOCAL_SENSOR));
		}
		if (sysStatus.isChanged(sysStatus.NTP)){
			drawNtpStatus(sysStatus.get(sysStatus.NTP));
		}		
		if (sysStatus.isChanged(sysStatus.TS_UPDATE)){
			drawThingSpeakUpdateStatus(sysStatus.get(sysStatus.TS_UPDATE));
		}
		if (sysStatus.isChanged(sysStatus.TS_GET)){
			drawThingSpeakGetStatus(sysStatus.get(sysStatus.TS_GET));
		}
	}

	if (resetStatus){
		sysStatus.resetChangeBits();
	}	
}

void WsnGui::redrawStatusBar(){
	if (sysStatus.getInt(sysStatus.WIFI) != -1){
		drawWifiStatus(sysStatus.get(sysStatus.WIFI));
	}
	if (sysStatus.getInt(sysStatus.RADIO) != -1){
		drawRadioStatus(sysStatus.get(sysStatus.RADIO));
	}
	if (sysStatus.getInt(sysStatus.LOCAL_SENSOR) != -1){
		drawSensorStatus(sysStatus.get(sysStatus.LOCAL_SENSOR));
	}
	if (sysStatus.getInt(sysStatus.NTP) != -1){
		drawNtpStatus(sysStatus.get(sysStatus.NTP));
	}		
	if (sysStatus.getInt(sysStatus.TS_UPDATE) != -1){
		drawThingSpeakUpdateStatus(sysStatus.get(sysStatus.TS_UPDATE));
	}
	if (sysStatus.getInt(sysStatus.TS_GET) != -1){
		drawThingSpeakGetStatus(sysStatus.get(sysStatus.TS_GET));
	}
}


void WsnGui::initMainScreen(){
	tft->setRotation(0);
	tft->setTextDatum(ML_DATUM);

	tft->fillRect(0,		0,		240,	24,	COLOR_BG_STATUSBAR);
	tft->fillRect(0,		24,	240,	100,	COLOR_BG_CLOCK);	

	tft->fillRect(0,		124,	120,	98,	COLOR_BG_BLOCK1);
	tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK1);
	tft->drawString("livingroom", 4, 134, 2);

	tft->fillRect(120,	124,	120,	98,	COLOR_BG_BLOCK2);
	tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK2);
	tft->drawString("child's room", 124, 134, 2);

	tft->fillRect(0,		222,	120,	98,	COLOR_BG_BLOCK2);
	tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK2);
	tft->drawString("outdoor", 4, 232, 2);

	tft->fillRect(120,	222,	120,	98,	COLOR_BG_BLOCK1);
	tft->setTextColor(TFT_DARKGREY, COLOR_BG_BLOCK1);
	tft->drawString("atm.pressure", 124, 232, 2);

}

void WsnGui::displaySensorData(const int8_t sensorID){
	tft->setTextPadding(100);
	switch (sensorID){
		case 0 :
			tft->setTextDatum(MC_DATUM);  
			tft->setTextColor(TFT_GREEN, COLOR_BG_BLOCK1);
			tft->setFreeFont(CF_OL32);
			tft->drawString(sensorDataCache.getTemperature(0), 60, 159, 1);
			tft->setFreeFont(CF_ORB11);
			tft->drawString(sensorDataCache.getHumidity(0), 60, 194, 1);

			tft->drawString(sensorDataCache.getPressure(0), 180, 257, 1);
			break;

		case 6 :
			tft->setTextDatum(MC_DATUM);  
			tft->setTextColor(TFT_GREEN, COLOR_BG_BLOCK2);  
			tft->setFreeFont(CF_OL32);
			tft->drawString(sensorDataCache.getTemperature(6), 180, 159, 1);
			tft->setFreeFont(CF_ORB11);
			tft->drawString(sensorDataCache.getHumidity(6), 180, 194, 1);
			break;

		case 1 :
			tft->setTextDatum(MC_DATUM);  
			tft->setTextColor(TFT_GREEN, COLOR_BG_BLOCK2);
			tft->setFreeFont(CF_OL32);
			tft->drawString(sensorDataCache.getTemperature(1), 60, 257, 1);
			tft->setFreeFont(CF_ORB11);
			tft->drawString(sensorDataCache.getHumidity(1), 60, 292, 1);

			break;
	}
	tft->setTextPadding(0);

/*
	//B2********************************************************  
	bx = b2x;
	by = b2y;


	tft.setTextDatum(MC_DATUM);  
	tft.setTextColor(TFT_GREEN,COLOR2);  
	tft.setFreeFont(CF_OL32);
	tft.drawString("21.1", bx+60, by+35, 1);
	//tft.setFreeFont(CF_OL24);
	tft.setFreeFont(CF_ORB);

	tft.drawString("56.8", bx+60, by+70, 1);

	//B3********************************************************
	bx = b3x;
	by = b3y;


	tft.setTextDatum(MC_DATUM);  
	tft.setTextColor(TFT_GREEN,COLOR2);
	tft.setFreeFont(CF_OL32);
	tft.drawString("-5.5", bx+60, by+35, 1);
	tft.setFreeFont(CF_OL24);
	tft.drawString("13.8", bx+60, by+70, 1);

	//B4******************************************************** 
	bx = b4x;
	by = b4y;


	tft.setTextDatum(MC_DATUM);
	tft.setTextColor(TFT_GREEN,COLOR1);
	tft.setFreeFont(CF_OL24);
	tft.drawString("1015", bx+60, by+34, 1);
*/
}

void WsnGui::displayClock(){
	char tftClock[6];
	sprintf(tftClock, "%u:%02u", hour(), minute());
 	tft->setTextColor(TFT_GREEN,TFT_BLACK);
	tft->setTextDatum(MC_DATUM);  
	tft->drawString(tftClock, 120, 60, 7);
}

void WsnGui::displayDate(){
	char tftDate[6];
	char tftDOW[5];
	sprintf(tftDate, "%02u.%02u", month(), day());
	strncpy(tftDOW, dayShortNames + (weekday() * 4), 4);
	tftDOW[4] = 0;

	tft->setTextColor(TFT_DARKGREY,TFT_BLACK);
	tft->setFreeFont(CF_ORB11);
	tft->setTextDatum(ML_DATUM);
	tft->drawString(tftDate, 4, 100, 1);
	tft->setTextDatum(MR_DATUM);
	tft->drawString(tftDOW, 237, 100, 1);

}

void WsnGui::displaySensorDump(){
	int posY = 25;
	int lh = 14;
	char buff[64];

	tft->setRotation(1);
	tft->fillScreen(TFT_BLACK);
	tft->setTextColor(TFT_GREEN,TFT_BLACK);
	tft->setTextDatum(ML_DATUM);  
	tft->setTextFont(0);
 
   tft->drawString(sensorDataCache.dumpTitle, 0, posY, 1);
  	posY = posY + lh;
   tft->drawString(sensorDataCache.dumpHeader, 0, posY, 1);
  	posY = posY + lh;

	for(int8_t i = 0; i < sensorDataCache.size(); i++){
		sensorDataCache.getDumpRow(i, buff);
		tft->drawString(buff, 0, posY, 1);
		posY = posY + lh;
	}	

   tft->drawString(sensorDataCache.dumpFooter, 0, posY, 1);
 
}