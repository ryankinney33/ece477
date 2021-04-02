/*
	The purpose of this program is to do something cool
	and related to part A. Part A was about measuring a
	100 Hz square wave on pin 0. The source of the square
	wave is an ATMEGA88PA and the reason for these programs
	is to calibrate its internal RC oscillator.

	What this program does is measure the frequency of the wave,
	and adjust the offset value stored in the AVR's EEPROM to
	increase or decrease the frequency. This program does not know
	what the initial offset is, so at program run-time, the offset
	value is set to 0 before the measurement and calibration begins.

	To calibrate, the following is done in a loop.
	  1. Read the frequency
	  2. Increase/Decrease the offset
	  3. Flash the offset into the EEPROM
	  4. Reset the AVR
	The program ends when the frequency crosses 100 Hz (either
	goes from less than 100Hz to more than or vice-versa)

	This program calls avrdude to flash the two EEPROM bytes.
	The config file used is /home/pi/avrdude_gpio.conf and the
	programmer used is the one using the Pi's GPIO pins. A shortcoming
	of this is that requires root access to export/unexport the GPIO
	pins, so this program must be run with root access (e.g sudo ./lab5b)

	Author: Ryan Kinney
	Ece 477 - Spring 2021
	April 2, 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

// used for frequency calculations
#define SAMPLE_C  50

// pin assignments
#define RESET    26
#define WAVE_IN  0

// This function initializes the GPIO pins
// Pin RESET as digital output
// Pin WAVE_IN as pull-down input
void gpio_init();

// Resets the AVR
void AVR_reset();

// Gets the frequency of the square wave on WAVE_IN
double get_freq();

// Calls get_freq and adjusts the EEPROM of the AVR to calibrate the clock
int adjust_freq();

void EEPROM_set(uint8_t byte1, uint8_t byte2);

int main(int argc, char* argv[]){
	// Check if the user is running as root
	// the avrdude program will fail to export GPIO pins to program without root access
	if(geteuid() != 0){
		printf("Please run this program as root.\n");
		printf("For example: sudo %s\n",argv[0]);
		exit(1);
	}

	gpio_init();

	// start by setting offset to 0,0
	EEPROM_set(0,0);

	while(adjust_freq());

	exit(0);
}

void gpio_init(){
	wiringPiSetup(); // set up wiring pi

	// set pin WAVE_IN as pull-down input
	pinMode(WAVE_IN,INPUT);
	pullUpDnControl(WAVE_IN,PUD_DOWN);

	// set pin RESET as output (for resetting the AVR)
	pinMode(RESET,OUTPUT);
	digitalWrite(RESET,HIGH);
}

// Resets the AVR by sending a low-pulse on pin 26
void AVR_reset(){
	digitalWrite(RESET,LOW);
	delay(1);
	digitalWrite(RESET,HIGH);
	delay(100); // delay 100 ms to ensure the AVR reaches a steady state before moving on
}

double get_freq(){
	// used for determining the time elapsed between two rising edges of the signal
	clock_t timer[2]; // timer[0] is the start of the period, timer[1] is the end
	double time[SAMPLE_C]; // the frequency is 1/time

	for(int j = 0; j < SAMPLE_C; ++j){
		// get the two times
		for(int i = 0; i < 2; ++i){
			int riseEdge = 0; // a flag for whether or not a rising edge was found
			int currentState = 1, previousState = 1; // current and previous input states
			// note, these are both initialized to 1 in order to ignore the initial state of the input pin
			// when the program is started (if the input starts high, that will not count as a rising edge

			while(!riseEdge){
				// update the current and previous states
				previousState = currentState;
				currentState = digitalRead(WAVE_IN);
				riseEdge = currentState && !previousState; // check for rising edge
			}
			timer[i] = clock(); // set the time
		}
		// calculate the elapsed time and save it in the array
		time[j] = ((double)(timer[1]-timer[0]))/CLOCKS_PER_SEC;
	}

	// find the average frequency of the wave
	double sum = 0.0; // start by finding the sum of the times
	for(int i = 0; i < SAMPLE_C; ++i)
		sum += time[i];

	return ((double)SAMPLE_C/sum);
}

int adjust_freq(){
	static struct offSetNumber{ // the initial offset is 0
		int n : 9;
	} offset = {0};

	static double previous = 0.0;

	// get and print the frequency
	double frequency = get_freq();

	// just for initializing
	if(previous == 0.0)
		previous = frequency;

	// return if the 100 Hz mark is crossed
	// this basically stops the frequency from bouncing around 100 Hz forever
	if((previous > 100.0 && frequency < 100.0) || (previous < 100.0 && frequency > 100.0)){
		printf("Final frequency is %lf Hz\n", frequency);
		return 0;
	}

	printf("Current frequency is %lf Hz, ", frequency);

	// increase or decrease frequency (with some bounds-checkings)
	if(frequency < 100.0){
		offset.n++; // increase the frequency
		if(offset.n == -256){ // check for overflow
			offset.n = 255;
			printf("cannot increase clock speed any further.\n");
		}else
			printf("increasing clock speed.\n");
	}else{
		offset.n--; // decrease the frequency
		if(offset.n == -256){ // minimum offset value is -255, prevent it from going lower
			offset.n = -255;
			printf("cannot decrease clock speed any further.\n");
		}else
			printf("decreasing clock speed.\n");
	}

	// get the sign and magnitude of the offset
	uint8_t sgn = (offset.n>>8) & 1;
	uint8_t mag = (sgn)? -offset.n : offset.n;

	// finally, set the EEPROM bytes
	EEPROM_set(mag,sgn);

	previous = frequency;

	return 1; // frequency needs to get closer
}

void EEPROM_set(uint8_t byte1, uint8_t byte2){
	char avrcommand[90];
	sprintf(avrcommand,"avrdude -C /home/pi/avrdude_gpio.conf -c pi_1 -p m88p -U eeprom:w:%x,%x:m 2> /dev/null",byte1,byte2);

	// easy way to check if the configuration file exists
	FILE* temp = fopen("/home/pi/avrdude_gpio.conf","r");
	if(temp == NULL){
		printf("Error: \"/home/pi/avrdude_gpio.conf\" does not exist.\n");
		exit(1);
	}
	fclose(temp);

	// flash the EEPROM with avrdude
	system(avrcommand);
	delay(10); // short delay
	AVR_reset(); // reset the AVR
}
