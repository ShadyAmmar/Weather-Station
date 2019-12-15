/*
by Shady Ammar
Date : 12_8_2019
--------------------
Wind Speed(Anemometer) and Wind Vane(Wind direction) Library 
*/

#ifndef WINDANEM

#include <Arduino.h>
#define VOLTerror 0.01

class WindAnem{
	
	private:
		int analogPin;		//	Wind Vane analog pin JP2/1st pin
		int anemometerPin;	//	Anemometer pin that support external interrupt
		static volatile unsigned long Rotations; // cup rotation counter used in interrupt routine
		static volatile unsigned long ContactBounceTime; // Timer to avoid contact bounce in interrupt routine
		float WindSpeed; // speed Km/hr
		float dir;
		
		float fmap(float x, float in_min, float in_max, float out_min, float out_max);
		
	public:	
		WindAnem(int analog_pin,int digital_pin);
		float getWindDirection();
		float getWindSpeed();
		static void anemISR();
		
	
};

#endif