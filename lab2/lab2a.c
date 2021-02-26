/*

The purpose of this program is to accept a single command line argument.
This command line argument is an integer constant in decimal hex or octal.
8 LEDs are attached to pins (listed in comments below) and will be lit up
(or shut off) according to the binary representation of that number.

Author: Ryan Kinney
ECE 477 - Spring 2021
February 25, 2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// subroutine for initializing the LEDs and setting the states
void set_leds(int num);

// Accepts a command line argument and lights up the LEDs accordingly
int main(int argc, char* argv[]){
	// we want to receive 1 command line argument, so argc should be 2
	if(argc != 2){
		printf("Error: Expected 1 argument, received %d.\n", argc-1);
		printf("Usage is %s number, where number is an integer constant.\n",argv[0]);
		set_leds(0);
		exit(1);
	}

	// next, we want to extract the integer value into an int
	// program accepts decimal, octal, and hex
	unsigned int number;
	if(!sscanf(argv[1],"0%o",&number)){
		// sscanf failed to read an octal number, check for hex
		if(!sscanf(argv[1],"0x%x",&number)){
			// sscanf failed to read a hexadecimal number, check for decimal
			if(!sscanf(argv[1],"%u",&number)){
				// something that was not a number was entered
				printf("Error: Illegal input.\n");
				printf("What was entered: %s\nExpected a number like 255, 0xFF, or 0377.\n",argv[1]);
				set_leds(0); // illegal input, turn off LEDs
				exit(2);
			}
		}
	}

	// next, make sure that the number was in range
	if(number > 0xFF){ // only 0-0xFF is supported (0-255, 0-0377)
		printf("Error: Input out of range.\n");
		printf("The input must be in the range of 0-0xFF, 0-255, or 0-0377.\n");
		set_leds(0); // illegal input, turn off LEDs
		exit(3);
	}

	// finally, set the LEDs to the output specified by number
	set_leds(number);

	exit(0);
}

// loops through the 8 LED pins, sets them to output
// then sets them high or low, depending on the parameter num
void set_leds(int num){
	wiringPiSetup();

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
		// iterate over WiringPi pins 0-7, setting them to output
		pinMode(i,OUTPUT);

		// set each LED according to the value of its corresponding bit
		// to get the value of the current bit, do num&(1<<i)
		// if that evaluates to true, set LED to high, else low
		digitalWrite(i, (num&(1<<i))? HIGH:LOW);
	}
}
