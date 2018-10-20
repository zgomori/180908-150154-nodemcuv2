#ifndef WSN_GUI_H
	#define WSN_GUI_H
	
	#include "Arduino.h" 
    #include "TFT_eSPI.h"
    #include "icons.h"

	class WsnGui{
		private:
            TFT_eSPI *tft;

		public:
			WsnGui(TFT_eSPI *tft);
            void test();
    };    
#endif
