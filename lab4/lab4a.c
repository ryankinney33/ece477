#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LEFT 1
#define RIGHT -1

void gpio_init();
void set_leds(int num);

int main(){
	gpio_init();

	// flags for button pressed status
	int previous=0, current=0;
	int direction = LEFT;

	int LEDstate = 0;

	int delayNum = 256;

	while(1){
		// set the LEDs
		set_leds(LEDstate);

		// wait the delay
		for(int i = 0; i < delayNum; ++i){
			delay(1);
		}

		// update the LED according to the direction
		LEDstate += direction;

		if(LEDstate < 0 || LEDstate > 7){
			// place the LED back in range
			LEDstate = (direction==LEFT)? 0:7;
		}
	}

	exit(0);
}

void gpio_init(){
	wiringPiSetup();

	for(int i = 0; i < 8; ++i){
		pinMode(i,OUTPUT);
	}

	// inputs on WiringPi 21 and 22
	pinMode(21,INPUT);
	pinMode(22,INPUT);

	// use internal pull down resistors
	pullUpDnControl(21,PUD_DOWN);
	pullUpDnControl(22,PUD_DOWN);
}

void set_leds(int num){
	for(int i = 0; i < 8; ++i){
		digitalWrite(i,LOW);
	}
	digitalWrite(num,HIGH);
}
