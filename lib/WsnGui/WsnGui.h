#ifndef WSN_GUI_H
   #define WSN_GUI_H
	
   #include "Arduino.h" 
   #include "TFT_eSPI.h"
	#include "TftUtil.h"
	#include "WsnSystemStatus.h"
   #include "icons.h"


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

			uint16_t cpIconNormal[16];
			uint16_t cpIconError[16];
		public:
         WsnGui(TFT_eSPI *tft);
      	void drawBackground();
			void drawWifiStatus(bool status);
			void drawRadioStatus(bool status);
			void drawSensorStatus(bool status);
			void refreshStatusBar(WsnSystemStatus &sysStat);
    };    
#endif
