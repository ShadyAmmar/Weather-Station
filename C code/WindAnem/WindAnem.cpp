/*
by Shady Ammar
Date : 12_8_2019
*/

#include "WindAnem.h"

volatile unsigned long WindAnem::ContactBounceTime = 0;
volatile unsigned long WindAnem::Rotations = 0;

WindAnem::WindAnem(int analog_pin,int digital_pin){
	analogPin = analog_pin;
	anemometerPin = digital_pin;
	
	pinMode(anemometerPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(anemometerPin), anemISR, FALLING);
}

float WindAnem::getWindDirection(){
	float adcValue = analogRead(analogPin);
	float volt = fmap(adcValue,0,1023.0,0,5)/1.52;
	if(volt > 2.53-VOLTerror && volt < 2.53+VOLTerror){
		dir = 0; 
	}else if(volt > 1.31-VOLTerror && volt < 1.31+VOLTerror){
		dir = 22.5;
	}else if(volt > 1.49-VOLTerror && volt < 1.49+VOLTerror){
		dir = 45;
	}else if(volt > 0.27-VOLTerror && volt < 0.27+VOLTerror){
		dir = 67.5;
	}else if(volt > 0.3-VOLTerror && volt < 0.3+VOLTerror){
		dir = 90;
	}else if(volt > 0.21-VOLTerror && volt < 0.21+VOLTerror){
		dir = 112.5;
	}else if(volt > 0.59-VOLTerror && volt < 0.59+VOLTerror){
		dir = 135;
	}else if(volt > 0.41-VOLTerror && volt < 0.41+VOLTerror){
		dir = 157.5;
	}else if(volt > 0.92-VOLTerror && volt < 0.92+VOLTerror){
		dir = 180;
	}else if(volt > 0.79-VOLTerror && volt < 0.79+VOLTerror){
		dir = 202.5;
	}else if(volt > 2.03-VOLTerror && volt < 2.03+VOLTerror){
		dir = 225;
	}else if(volt > 1.93-VOLTerror && volt < 1.93+VOLTerror){
		dir = 247.5;
	}else if(volt > 3.05-VOLTerror && volt < 3.05+VOLTerror){
		dir = 270;
	}else if(volt > 2.67-VOLTerror && volt < 2.67+VOLTerror){
		dir = 292.5;
	}else if(volt > 2.86-VOLTerror && volt < 2.86+VOLTerror){
		dir = 315;
	}else if(volt > 2.26-VOLTerror && volt < 2.26+VOLTerror){
		dir = 337.5;
	}else{
    
	}
	
	return dir;
}

float WindAnem::getWindSpeed(){
	Rotations = 0; // Set Rotations count to 0 ready for calculations

	sei(); // Enables interrupts
	delay(1000); // Wait 1 seconds to average
	cli(); // Disable interrupts
	
	//From Datasheet
	WindSpeed = Rotations * 2.4;
	
	return WindSpeed;
}

void WindAnem::anemISR(){
	if ((millis() - ContactBounceTime) > 15 ) { // debounce the switch contact.
	Rotations++;
	ContactBounceTime = millis();
	}
}

float WindAnem::fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}