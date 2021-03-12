/*
	The purpose of this program is to rotate a single lit LED
	across a row of LEDs. Uses two pushbuttons to control the
	delay between the LED changes. Button A reduces the delay
	and button B doubles it. When at the max or minimum delay,
	the buttons reverse the direction of the LED.

	Author: Ryan Kinney
	ECE 477 - Spring 2021
	March 12, 2021
*/

#include <wiringPi.h>
#include "gpio.h"

// Macros for controlling the LED direction
#define LEFT 1
#define RIGHT -1

int main(){
	// create button structs and set up the GPIO pins
	button A,B;
	A.pin = 21;
	A.previous = A.current = 0;
	B.pin = 22;
	B.previous = B.current = 0;
	gpio_init(&A,&B);

	// set up variables for lighting the LEDs
	int direction = LEFT;
	int LEDstate = 0;
	int delayNum = 1024;

	while(1){
		// set the LEDs
		set_leds(LEDstate);

		// wait the delay
		for(int i = 0; i < delayNum; ++i){
			// get input from the user
			get_input(&A);
			if(A.current && !A.previous){ // button A was just pressed
				// Decrease delay
				delayNum >>= 1;
				if(delayNum < 32){ // Reverse the direction if necessary
					delayNum = 32;
					direction = -direction;
				}
			}else{
				get_input(&B);
				if(B.current && !B.previous){ // button B was just pressed
					// Increase delay
					delayNum <<= 1;
					if(delayNum > 1024){ // reverse direction if necessary
						delayNum = 1024;
						direction = -direction;
					}
				}
			}
			delay(1);
		}

		// update the LED according to the direction
		LEDstate += direction;

		if(LEDstate < 0 || LEDstate > 7){
			// place the LED back in range
			LEDstate = (direction==LEFT)? 0:7;
		}
	}

	return 0;
}
