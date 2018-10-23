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
			TftUtil(TFT_eSPI *tft);
			void generateColorPalette16(uint16_t *colorPalette,  uint16_t fgColor, uint16_t bgColor);
			void drawIcon4(const icon_t *icon, uint16_t *colorPalette, int16_t x, int16_t y);
			void drawIcon4(const icon_t *icon, uint16_t fgColor, uint16_t bgColor, int16_t x, int16_t y);
    };    
#endif