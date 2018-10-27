#ifndef TFT_UTIL_H
	#define TFT_UTIL_H
	
	#include "Arduino.h" 
	#include "TFT_eSPI.h"

	typedef struct{
		const uint8_t width;
		const uint8_t height;
		const uint8_t *rawData;
	} icon_t;
	
	class TftUtil{
		private:
         TFT_eSPI *tft;

		public:
			TftUtil(TFT_eSPI *_tft);
         TftUtil();
         void init(TFT_eSPI *_tft);
			void generateColorPalette16(const uint16_t fgColor, const uint16_t bgColor, uint16_t *colorPaletteOut);
			void drawIcon4(const icon_t *icon, const uint16_t *colorPalette, const int16_t x, const int16_t y);
			void drawIcon4(const icon_t *icon, const uint16_t fgColor, const uint16_t bgColor, const int16_t x, const int16_t y);
    };    
#endif