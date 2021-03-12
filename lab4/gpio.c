/*
	This file contains the function definitions
	for the functions described in gpio.h

	Author: Ryan Kinney
	ECE 477 - Spring 2021
	March 12, 2021
*/

#include "gpio.h"
#include <wiringPi.h>

// Sets the pins listed in the button struct as
// input and sets it to use the pull up resistor
// Also sets pins 0-7 as outputs for lighting the LEDs
void gpio_init(button* A, button* B){
	wiringPiSetup();

	for(int i = 0; i < 8; ++i){
		pinMode(i,OUTPUT);
	}

	// inputs on the pins in the structs
	pinMode(A->pin,INPUT);
	pinMode(B->pin,INPUT);

	// use internal pull down resistors
	pullUpDnControl(A->pin,PUD_UP);
	pullUpDnControl(B->pin,PUD_UP);
}

// Lights up the LED specified by num.
// For example, if num is 5, LED 5 will be lit up
void set_leds(int num){
	// first, turn all the LEDs off
	for(int i = 0; i < 8; ++i){
		digitalWrite(i,LOW);
	}
	// turn on the LED for pin num
	digitalWrite(num,HIGH);
}

// Saves the previous input and gets the new input
void get_input(button* s){
	s->previous = s->current;
	s->current = !digitalRead(s->pin);
}
