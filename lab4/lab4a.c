#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>


void gpio_init();
void set_leds(int num);

int main(){
	gpio_init();




	while(1){
		for(int i = 1; i < 129; i <<= 1){
			set_leds(i);
			delay(32);
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
	for(int i = 0; i < num; ++i){
		if(num&(1<<i)){
			digitalWrite(i,HIGH);
		}else{
			digitalWrite(i,LOW);
		}
	}
}
