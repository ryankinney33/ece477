#include "gpio.h"
#include <stdio.h>
#include <wiringPi.h>

// Macros for "moving" the LED
#define LEFT  1
#define RIGHT -1
#define SERVE 0

// Macros for the state of the game
#define LSERVE -1
#define RSERVE 1

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
	int state = RSERVE;
	int litLED = 0;

	while(1){
		// start by lighting the correct LED
		set_leds(litLED);


		// move the LED according to the direction
		litLED += direction;

		if(litLED > 7 || litLED < 0){
			litLED=0;
		}

		delay(256); // delay about 1/8 of second
	}
}
