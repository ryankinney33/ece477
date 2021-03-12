#include <wiringPi.h>
#include "gpio.h"

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

		int Acheck_delay = 0, Bcheck_delay = 0;

		// wait the delay
		for(int i = 0; i < delayNum; ++i){
			// get input from the user
			get_input(&A);
			if(A.current && !A.previous && Acheck_delay <= 0){ // button A was just pressed
				// Decrease delay
				delayNum >>= 1;
				if(delayNum < 32){ // Reverse the direction if necessary
					delayNum = 32;
					direction = -direction;
				}
				Acheck_delay = 20;
			}else{
				get_input(&B);
				if(B.current && !B.previous && Bcheck_delay <= 0){ // button B was just pressed
					// Increase delay
					delayNum <<= 1;
					if(delayNum > 1024){ // reverse direction if necessary
						delayNum = 1024;
						direction = -direction;
					}
				Bcheck_delay = 20;
				}
			}
			delay(1);
			--Acheck_delay;
			--Bcheck_delay;
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
