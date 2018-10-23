#include "TftUtil.h"

TftUtil::TftUtil(TFT_eSPI *_tft){
	this->tft = _tft;
}

void TftUtil::generateColorPalette16(uint16_t *colorPalette,  uint16_t fgColor, uint16_t bgColor){
	uint16_t fgR = ((fgColor >> 10) & 0x3E) + 1;
	uint16_t fgG = ((fgColor >>  4) & 0x7E) + 1;
	uint16_t fgB = ((fgColor <<  1) & 0x3E) + 1;

	uint16_t bgR = ((bgColor >> 10) & 0x3E) + 1;
	uint16_t bgG = ((bgColor >>  4) & 0x7E) + 1;
	uint16_t bgB = ((bgColor <<  1) & 0x3E) + 1;

	//uint8_t alpha; 

	colorPalette[0] = bgColor;
	for (uint8_t i = 1; i < 15; i++){
		//alpha = i * 16;
		uint16_t r = (((fgR * i * 16) + (bgR * (255 - (i * 16)))) >> 9);
		uint16_t g = (((fgG * i * 16) + (bgG * (255 - (i * 16)))) >> 9);
		uint16_t b = (((fgB * i * 16) + (bgB * (255 - (i * 16)))) >> 9);
		colorPalette[i] = (r << 11) | (g << 5) | (b << 0);        
	}
	colorPalette[15] = fgColor;
}

void TftUtil::drawIcon4(const icon_t *icon, uint16_t *colorPalette, int16_t x, int16_t y){
	tft->setWindow(x, y, x + icon->width - 1, y + icon->height - 1);
	uint16_t  writeBuffer[64];   
	uint8_t  buffers = (icon->width * icon->height) / 64;

	for (int i = 0; i < buffers; i++) {
		for (int j = 0; j < 32; j++) {
			writeBuffer[j*2] = colorPalette[icon->rawData[i * 32 + j] >> 4];
			writeBuffer[(j*2)+1] = colorPalette[icon->rawData[i * 32 + j] & 0x0F];  
		}
		tft->pushColors(writeBuffer, 64);   
	}

	uint8_t restBytes = ((icon->height * icon->width) % 64)/2;
	if (restBytes) {
		for (int i = 0; i < restBytes; i++){
			writeBuffer[i*2] = colorPalette[icon->rawData[buffers * 32 + i] >> 4];
			writeBuffer[(i*2)+1] = colorPalette[icon->rawData[buffers * 32 + i] & 0x0F];
		}
		tft->pushColors(writeBuffer, restBytes*2);
	}
}

void TftUtil::drawIcon4(const icon_t *icon, uint16_t fgColor, uint16_t bgColor, int16_t x, int16_t y){
	uint16_t _colorPalette16[16];

	this->generateColorPalette16(_colorPalette16, fgColor, bgColor);
	this->drawIcon4(icon, _colorPalette16, x, y);
}	