#ifndef TFT_BARCHART_H
#define BARCHART_H

#include "TFT_eSPI.h"

template<class T>
struct barChartConfig_t{
	uint16_t origoX;
	uint16_t origoY;
	int16_t scaleLineOffset;
	uint16_t scaleLineWidth;
	uint16_t pixelPerUnit;
	uint16_t barWidth;
	uint16_t barPadding;
	uint16_t numberOfBars;
	T scaleUnit;
	T minValue;
	T maxValue;
	uint32_t bgColor;
	uint32_t scaleColor;
	uint32_t barColor;
};


template<class T> 
class TftBarChart{
	private:
		TFT_eSPI *tft;
		barChartConfig_t<T> cfg;
	public:
		TftBarChart(){
		}
/*
		TftBarChart(TFT_eSPI *tft, barChartConfig_t<T> config){
			this->tft = tft;
			this->cfg = config;
		}
*/

		void init(TFT_eSPI *tft, barChartConfig_t<T> config){
			this->tft = tft;
			this->cfg = config;
		}

		void drawScale(){
			//clear
			tft->fillRect(	cfg.origoX + cfg.scaleLineOffset, 
								cfg.origoY - ((cfg.maxValue / cfg.scaleUnit) * cfg.pixelPerUnit), 
//								((cfg.barPadding + cfg.barWidth) * cfg.numberOfBars) + (cfg.barPadding - 1),
								abs(cfg.scaleLineOffset) + cfg.scaleLineWidth + cfg.barPadding + ((cfg.barPadding + cfg.barWidth) * cfg.numberOfBars) + cfg.barPadding,
								(((cfg.maxValue / cfg.scaleUnit) - (cfg.minValue / cfg.scaleUnit)) * cfg.pixelPerUnit) + 1,
								 cfg.bgColor
							 ); 
			// draw X line 
			tft->drawFastHLine(	cfg.origoX, 
										cfg.origoY, 
//										((cfg.barPadding + cfg.barWidth) * cfg.numberOfBars) + (cfg.barPadding-1), 
										cfg.scaleLineWidth + cfg.barPadding + ((cfg.barPadding + cfg.barWidth) * cfg.numberOfBars) + cfg.barPadding, 
										cfg.scaleColor
									);
			// draw Y line
			tft->drawFastVLine(cfg.origoX, cfg.origoY - ((cfg.maxValue / cfg.scaleUnit) * cfg.pixelPerUnit), (((cfg.maxValue / cfg.scaleUnit) - (cfg.minValue / cfg.scaleUnit)) * cfg.pixelPerUnit) + 1, cfg.scaleColor);
			// draw Y scaling
			bool before = true;
			for(uint16_t i = cfg.origoY - ((cfg.maxValue / cfg.scaleUnit) * cfg.pixelPerUnit); i <= cfg.origoY + (abs(cfg.minValue / cfg.scaleUnit) * cfg.pixelPerUnit); i+= cfg.pixelPerUnit){
				if (before){	
    				tft->drawFastHLine(cfg.origoX + cfg.scaleLineOffset, i, cfg.scaleLineWidth, cfg.scaleColor);
				}
				else{
					tft->drawFastHLine(cfg.origoX+1 , i, cfg.scaleLineWidth, cfg.scaleColor);
				}	
				before = !before; 
  			}
 		}

		void drawBar(uint16_t barIndex, T value, bool clearArea){
			// wrong index
			if (barIndex >= cfg.numberOfBars){
				return;
			}

			if (value < cfg.minValue){
				value = cfg.minValue;
			}
			if (value > cfg.maxValue){
				value = cfg.maxValue;
			} 

			if (value > 0){
				tft->fillRect(cfg.origoX + cfg.scaleLineWidth + cfg.barPadding + ((cfg.barPadding + cfg.barWidth) * barIndex) , cfg.origoY - ((value / cfg.scaleUnit) * cfg.pixelPerUnit), cfg.barWidth, ((value / cfg.scaleUnit) * cfg.pixelPerUnit), cfg.barColor);
			}
			else if (value < 0){
				tft->fillRect(cfg.origoX + cfg.scaleLineWidth + cfg.barPadding + ((cfg.barPadding + cfg.barWidth) * barIndex), cfg.origoY + 1, cfg.barWidth, abs(((value / cfg.scaleUnit) * cfg.pixelPerUnit)), cfg.barColor);
			}
			else{ // 0
			//    tft.fillRect(origoX+barPadding, origoY-2, barWidth, 5, TFT_GREEN);
				tft->drawFastHLine(cfg.origoX + cfg.scaleLineWidth + cfg.barPadding + ((cfg.barPadding + cfg.barWidth) * barIndex), cfg.origoY, cfg.barWidth, cfg.barColor);
			}
		} 

};	
#endif	
