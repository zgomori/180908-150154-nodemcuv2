#ifndef DATA_HISTORY_H
   #define DATA_HISTORY_H


template<class T, uint16_t SIZE> 
class DataHistory{
	private:
		T elementArray[SIZE] = {T()};
		uint16_t nextIdx = 0;
		uint16_t firstEmptyIdx = 0;  // used for min, max computing only
		uint8_t mode = DataHistory::FIFO;
	public:
		enum mode_enum_t{
			FIFO = 0,
			LIFO = 1
		};

		DataHistory(){}

		void setMode(mode_enum_t pMode){
			mode = pMode;
		}

		// returns true when last element added and nextIdx reset to 0 
		bool add(T newElemet){
			elementArray[nextIdx++] = newElemet;
			if (nextIdx == SIZE){
				nextIdx = 0;
			}
			if (firstEmptyIdx < SIZE){
				firstEmptyIdx++;
			}
			return (nextIdx == 0);
		}

		// indexed by time series
		// index 0 is the oldest element, index (SIZE-1) is the last added element

		inline T operator [] (uint16_t timeSeriesidx){
			if (timeSeriesidx > SIZE-1){
				return (T)0;
			}
			uint16_t arrIndex = nextIdx + timeSeriesidx;
			if(arrIndex > SIZE-1){
				arrIndex = arrIndex - SIZE; 	
			}
			return (mode == DataHistory::FIFO) ? elementArray[arrIndex] : elementArray[(SIZE-1) - arrIndex];
		}

		inline T getOldest(){
			return ((firstEmptyIdx < SIZE ? elementArray[0] : elementArray[nextIdx]));
		}

		uint16_t size(){
			return SIZE;	
		}

		void getMinMax(T &outMin, T &outMax){
			outMin = elementArray[0];
			outMax = elementArray[0];
			for(uint16_t i = 0; i < firstEmptyIdx; i++){
				if(elementArray[i] < outMin){
					outMin = elementArray[i];
				} 
				if(elementArray[i] > outMax){
					outMax = elementArray[i];
				}  	

			}
		}

		T getAvg(){
			T avg = T(0);
			for(uint16_t i = 0; i < firstEmptyIdx; i++){
				avg = avg + elementArray[i];
			}  	
			
			return avg / firstEmptyIdx; 
		}	

}; 


#endif	