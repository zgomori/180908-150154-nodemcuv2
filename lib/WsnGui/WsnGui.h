#ifndef WSN_GUI_H
   #define WSN_GUI_H
	
   #include "Arduino.h" 
   #include "TFT_eSPI.h"
	#include "TftUtil.h"
	#include <TimeLib.h>
	#include "WsnSystemStatus.h"
	#include "WsnSensorDataCache.h"
   #include "icons.h"
	#include <Fonts/Custom/orbitron_light_11_2.h>
  
	#define CF_OL32 &Orbitron_Light_32
	#define CF_ORB11 &orbitron_light11pt7b

	#define COLOR_BG_STATUSBAR 0x18C3
   #define COLOR_BG_CLOCK     0x0000
	#define COLOR_BG_BLOCK1		0x18C3
	#define COLOR_BG_BLOCK2		0x10A2
   #define COLOR_ICON_NORMAL	0x07E0
	#define COLOR_ICON_ERROR	0xF800


	class WsnGui{
		private:
         TFT_eSPI *tft;
			TftUtil tftUtil;

			uint8_t currentScreenId = 0;
			uint16_t cpIconNormal[16];
			uint16_t cpIconError[16];
//			const char *dayShortNames = "ErrSunMonTueWedThuFriSat"; 
			const char *dayShortNames = "Err  Vas HetKedd SzeCsüt Pen Szo"; 
		
		
		public:
      	enum screen_enum{
				SCR_MAIN = 0,
				SCR_MENU = 1,
				SCR_SENSOR_DUMP = 2,
				SCR_CONFIG = 3,
				SCR_STATUS = 4
			};
		
		
		   WsnGui(TFT_eSPI *tft);
			uint8_t getCurrentScreenId();
			void switchScreen(uint8_t screenId);
      	void initMainScreen();
			void drawWifiStatus(bool status);
			void drawRadioStatus(bool status);
			void drawSensorStatus(bool status);
			void drawNtpStatus(bool status);
			void drawThingSpeakUpdateStatus(bool status);
			void drawThingSpeakGetStatus(bool status);
			void updateStatusBar(bool resetStatus);
			void redrawStatusBar();
			void displaySensorData(const int8_t sensorID);
			void displayClock();
			void displayDate();
			void displaySensorDump();
    };    

extern WsnSensorDataCache sensorDataCache;
extern WsnSystemStatus sysStatus;
#endif
