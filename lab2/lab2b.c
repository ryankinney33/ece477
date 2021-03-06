/*

The purpose of this program is very similar to lab2a.c
It accepts multiple command line arguments that are integer constants
from 0-255 in decimal, 0-0xFF in hex, and 0-0377 in octal.
Each valid input will then light up the LEDs (this is repeated until the program is killed).

Author: Ryan Kinney
ECE 477 - Spring 2021
February 25, 2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>


// Sets up the pins with LEDs connected for outputting
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

// Sets the LEDs according to the binary representation
// of parameter num
void set_leds(unsigned int num){
	for(int i = 0; i < 8; ++i){
		digitalWrite(i,(num&(1<<i))? HIGH:LOW);
	}
}


// loops through the input arguments, filters out illegal inputs, and flashes the LEDs accordingly.
// example: if the user enters 5 valid numbers, loop through them, setting the LEDs
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
		// check for octal
		if(!sscanf(argv[i],"0%o",&number))

		// not octal, check for hex
		if(!sscanf(argv[i],"0x%x",&number))
		// not hex, check for decimal

		if(!sscanf(argv[i],"%u",&number))
			// invalid input entered, set number to an illegal value
			number = 0x100;

		// check validity of input
		if(number > 0xFF){
			// invalid input; complain, do not increment counter, do not save it
			printf("Error: Illegal input.\n");
			printf("What was entered: %s\n",argv[i]);
			printf("Expected an integer constant from 0-255 in decimal, 0-0xFF in hex, or 0-0377 in octal.\n\n");
		}else
			// save the input and increment the counter
			valid[size++] = number;
	}

	if(size == 0){
		// no valid inputs
		printf("Error: No valid inputs.\n");
		printf("Inputs are integer constants from 0-255 in decimal, hex, or octal.\n");
		exit(2);
	}

	// flash the LEDs
	while(1){
		// for each element in valid, set the LEDS to it, with a small delay
		for(int i = 0; i < size; i++){
			set_leds(valid[i]);
			delay(140);
		}
	}

	exit(0);
}
