/*

The purpose of this program is to read the one minute load average
in the file /proc/loadavg, convert it to a thermometer code, and light
up the 8 LEDs accordingly. If the read value is 4.0, all LEDs should be on.
Every time the load halves, an LED should be shut off until they are all off.

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

	for(int i = 0; i < 8; ++i)
		pinMode(i, OUTPUT);
}

// Reads the first number in /proc/loadavg and lights up the
// LEDs in a "thermometer code" to indicate the load
int main(){
	// program set up
	led_setup();
	FILE *fp;

	while(1){
		// open the loadavg file and check for error
		fp = fopen("/proc/loadavg","r");
		if(fp == NULL){
			perror("Error");
			exit(-1);
		}
		// read the loadAVG and find the position of the most significant bit
		double loadAVG;
		fscanf(fp,"%lf",&loadAVG);

		// since we want 4.0 in the loadavg to light up 8 LEDs
		// multiply loadAVG by 32 so that 4.0 maps to bit 7 of an int
		int temp = (int)(loadAVG*32.0);
		int numLEDs = 0;

		// finds the most significant 1, which will indicate how many
		// LEDs to light up (basically log_2)
		while(temp > 0){
			temp >>= 1;
			++numLEDs;
		}

		// numLEDs should be the number of LEDs to light up
		int i;
		for(i = 0; i < numLEDs && i < 8; ++i){ // light up LED 0-numLEDs
			digitalWrite(i,HIGH);
		}
		for(i = numLEDs; i < 8; ++i){ // turn off LED numLEDs-7
			digitalWrite(i,LOW);
		}

		// close the file (for reopening)
		fclose(fp);
		sleep(3);
	}

	exit(0);
}
