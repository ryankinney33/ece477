/*

The purpose of this program is to a number from a file, convert it to a thermometer code,
and light up the 8 LEDs accordingly. If the read value is 4.0, all LEDs should be on.
Every time the value halves, an LED should be shut off until they are all off.

Accepts a command line argument for the path to the file. If no argument is passed,
the program opens /proc/loadavg and reads the one minute average from it.

Author: Ryan Kinney
ECE 477 - Spring 2021
March 5, 2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>


// Sets up the pins with LEDs connected for outputting
void led_setup(){
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

	for(int i =0; i < 8; ++i){
		pinMode(i, OUTPUT);
	}
}

// The code for part B of the lab
// It works very similarly to part A, but accepts
// a command line argument for a file path to read instead of /proc/loadavg
// The specified file must begin with a double constant, e.g. the first value
// in the file must be a double.
int main(int argc, char* argv[]){
	// program set up
	led_setup();
	FILE *fp;

	if(argc > 2){
		printf("Error: Expected 0 or 1 argument, received %d\n",argc-1);
		printf("Usage is %s or %s filepath, where filepath is a path to a file",argv[0],argv[0]);
		exit(2);
	}

	while(1){
		if(argc==1) // no input arguments, default to /proc/loadavg
			fp = fopen("/proc/loadavg","r");
		else
			fp = fopen(argv[1],"r"); // open the path specified by the argument

		// check for error opening the file
		if(fp == NULL){
			perror("Error");
			exit(-1);
		}

		// the file specified should contain a double as its first number
		// scan the first number from the file
		double therm;
		if(fscanf(fp,"%lf",&therm) == 0){
			// could not read a number, print an error and exit
			printf("Error, the first number of the file should be a double constant\n");
			fclose(fp);
			exit(1);
		}

		// close the file, for now
		fclose(fp);

		// a double value is in therm, now find out how many LEDs to light up based on it
		int temp = (int)(therm*32.0);
		int numLEDs = 0;

		// find the most significant 1
		while(temp > 0){
			temp >>= 1;
			++numLEDs;
		}

		// light up them LEDs
		int i;
		for(i = 0; i<numLEDs && i < 8; ++i){
			digitalWrite(i,HIGH);
		}
		// turn off LEDs that should be off
		for(i = numLEDs; i<8; ++i){
			digitalWrite(i,LOW);
		}

		// sleep a bit
		sleep(3);
	}
}
