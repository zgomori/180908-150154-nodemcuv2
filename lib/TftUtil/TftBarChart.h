#ifndef TFT_BARCHART_H
	#define BARCHART_H


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
};


template<class T> 
class TftBarChart{
	private:
		barChartConfig_t<T> cfg;
	public:
		TftBarChart(){
		}

		TftBarChart(barChartConfig_t<T> config){
			cfg = config;
		}

		void drawScale(){

		}

		void drawBar(uint16_t barIndex, T value, bool clearArea){

		} 

};	
#endif	

/*

uint16_t origoX = 200;
uint16_t origoY = 250;

int16_t scaleLineOffset = -2;
uint16_t scaleLineWidth = 2;
//uint16_t pixelPerUnit = 4;
//uint16_t barWidth = 5;
uint16_t pixelPerUnit = 5;
uint16_t barWidth = 8;

uint16_t barPadding = 4;

uint16_t numberOfBars = 3;
float scaleUnit = 0.5;
float minValue = -2.5; //!!
float maxValue =  2.5; //!!


uint16_t barIndex = 0;
//clear
tft.fillRect(origoX+scaleLineOffset,origoY-((maxValue/scaleUnit)*pixelPerUnit), ((barPadding + barWidth) * numberOfBars) + (barPadding-1), (((maxValue/scaleUnit) - (minValue/scaleUnit)) * pixelPerUnit)+1, COLOR1); 

//for (int value= 5; value > -6; value-- ){
for (float value= 2.5; value >= -2.5; value-=2){
  
  tft.drawFastHLine(origoX+1, origoY, ((barPadding + barWidth) * numberOfBars) + (barPadding-1) , TFT_DARKGREY);
  tft.drawFastVLine(origoX, origoY-((maxValue/scaleUnit)*pixelPerUnit), (((maxValue/scaleUnit) - (minValue/scaleUnit)) * pixelPerUnit)+1, TFT_DARKGREY);

  for(uint16_t i=origoY-((maxValue/scaleUnit)*pixelPerUnit); i <= origoY+(abs(minValue/scaleUnit) * pixelPerUnit); i+=pixelPerUnit){
    tft.drawFastHLine(origoX+scaleLineOffset, i, scaleLineWidth, TFT_DARKGREY);
  }

  if (value > 0){
    tft.fillRect(origoX + barPadding + ((barPadding + barWidth) * barIndex) , origoY-((value/scaleUnit)*pixelPerUnit), barWidth, ((value/scaleUnit)*pixelPerUnit), TFT_GREEN);
  }
  else if (value < 0){
    tft.fillRect(origoX + barPadding + ((barPadding + barWidth) * barIndex), origoY+1, barWidth, abs(((value/scaleUnit)*pixelPerUnit)),TFT_GREEN);
  }
  else{ // 0
//    tft.fillRect(origoX+barPadding, origoY-2, barWidth, 5, TFT_GREEN);
    tft.drawFastHLine(origoX+barPadding + ((barPadding + barWidth) * barIndex), origoY, barWidth, TFT_GREEN);
  }
  barIndex++;
  // delay(2000);
}

*/