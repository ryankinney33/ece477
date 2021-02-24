#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

void LED_setup(){
	/* Pins used:
		LED 0: WiringPi pin 0, BCM GPIO17, Physical pin 17
		LED 1: WiringPi pin 1, BCM GPIO18, Physical pin 12
		LED 2: WiringPi pin 2, BCM GPIO27, Physical pin 13
		LED 3: WiringPi pin 3, BCM GPIO22, Physical pin 15
		LED 4: WiringPi pin 4, BCM GPIO23, Physical pin 16
		LED 5: WiringPi pin 5, BCM GPIO24, Physical pin 18
		LED 6: WiringPi pin 6, BCM GPIO25, Physical pin 22
		LED 7: WiringPi pin 7, BCM GPIO4,  Physical pin 7
	*/
	for(int i = 0; i < 8; ++i){
		pinMode(i,OUTPUT);
	}
}

int main(int argc, char* argv[]){
	// setup the required GPIO pins
	wiringPiSetup();
	LED_setup();

	return 0;
}
