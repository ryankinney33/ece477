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

void set_leds(unsigned int num){
	for(int i = 0; i < 8; ++i){
		digitalWrite(i,(num&(1<<i))? HIGH:LOW);
	}
}


// loop through the input arguments, flashing the LEDs accordingly
// example: if the user enters 5 valid numbers, loop through them, setting the LEDS
// to each one, with a small delay between them
int main(int argc, char* argv[]){
	// setup the required GPIO pins
	wiringPiSetup();
	LED_setup();

	// check for empty input
	if(argc == 1){
		printf("Error: Expected at least 1 argument, received none.\n");
		printf("Usage is %s number1 number2 number3 ..., where numberX is an integer constant.\n",argv[0]);
		set_leds(0);
		exit(1);
	}

	// the input is not empty, loop through the input to save the valid arguments
	// make an array to hold all the valid inputs
	unsigned int valid[argc-1];
	int size = 0; // the number of ints in valid
	unsigned int number; // the extracted number from the arguments

	for(int i = 1; i < argc; ++i){
		int test;
		// check for octal
		if(test = sscanf(argv[i],"0%o",&number));
		// not octal, check for hex
		else if(test = sscanf(argv[i],"0x%x",&number));
		// not hex, check for decimal
		else if(test = sscanf(argv[i],"%u",&number));

		// check validity of input
		if(test==0 || number > 0xFF){
			// invalid input, complain, do not increment counter, do not save it
			printf("Error: Illegal input.\n");
			printf("What was entered: %s\n",argv[i]);
			printf("Expected an integer constant from 0-255 in decimal, 0-0xFF in hex, or 0-0377 in octal.\n");
		}else
			// save the input and increment the counter
			valid[size++] = number;
	}

	// flash the LEDs
	while(1){
		// for each element in valid, set the LEDS to it, with a small delay
		for(int i = 0; i < size; i++){
			set_leds(valid[i]);
			delay(50);
		}
	}

	exit(0);
}
