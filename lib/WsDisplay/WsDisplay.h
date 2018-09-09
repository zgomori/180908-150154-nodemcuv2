#ifndef WS_DISPLAY_H
	#define WS_DISPLAY_H

	#include "Adafruit_ILI9341.h"

	#define MAX_DISPLAY_BLOCKS 10
	#define MAX_DATAFIELDS 15

	class WsDisplay{
		public:
			enum align_enum {
				LEFT = 'L',
				RIGHT = 'R',
				CENTER = 'C'
			};
    
	 	private:
			typedef struct{
				uint8_t   x;
				uint16_t  y;
				uint8_t   w;      
				uint16_t  h;
				uint16_t  fgColor;
				const GFXfont *font;      
				align_enum align;
				char     *dataPtr;
				int8_t   blockIdx = -1;
				void* fieldChain;   
			} DataField;

    		typedef struct{
				uint8_t   x;
				uint16_t  y;
				uint8_t   w;      
				uint16_t  h;
				uint16_t  bgColor;
				int8_t   screenIdx = -1;
				DataField* fieldChain;
			} DisplayBlock;

    		int8_t blockArrIdx = -1;   
    		int8_t dataFieldArrIdx = -1;  
  
    		Adafruit_ILI9341 *_tft;
    		DisplayBlock displayBlockArr[MAX_DISPLAY_BLOCKS];
    		DataField   dataFieldArr[MAX_DATAFIELDS];
    		int8_t currentScreenIdx = 0;   

		public:
			WsDisplay(Adafruit_ILI9341 *tft);
			uint8_t defineBlock(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t _h, uint16_t _bgColor, int8_t _screenIdx);
			uint8_t defineDataField(uint8_t _x, uint16_t _y, uint8_t _w, uint16_t  _h, uint16_t _fgColor, const GFXfont *_font, align_enum _align, char *_dataPtr, int8_t _blockIdx);
			void showData(WsDisplay::DataField* dataField, bool fill);
			void showData(int8_t fieldIdx, bool fill);
			void showBlock(int8_t blockIdx);
			void showBlock(int8_t blockIdx, bool fill);
			void showScreen();
			void switchScreen(int8_t screenIdx);
			int8_t getCurrentScreen();
	};

#endif // WS_DISPLAY_H