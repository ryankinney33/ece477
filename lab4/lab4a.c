#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>


void gpio_init();

int main(){
	gpio_init();









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
