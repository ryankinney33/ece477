#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>

// used for frequency calculations
#define SAMPLE_C  50
#define THRESH    0.25

// pin assignments
#define RESET    26
#define WAVE_IN  0
#define AVR_RDY  3
#define AVR_DIR  2

// This function initializes the GPIO pins
// Pins RESET and AVR_DIR as outputs
// Pins WAVE_IN and AVR_RDY as pull-down inputs
void gpio_init();

// Resets the AVR
void AVR_reset();

double get_freq();

int adjust_freq();

int main(){
	gpio_init();

	AVR_reset();

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

	// set pin AVR_DIR as output (for telling the AVR what direction the clock speed needs to move)
	pinMode(AVR_DIR,OUTPUT);
	digitalWrite(AVR_DIR,LOW);

	// set pin AVR_RDY as pull-down input (get status from AVR)
	pinMode(AVR_RDY,INPUT);
	pullUpDnControl(AVR_RDY,PUD_DOWN);
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
	// get and print the frequency
	double frequency = get_freq();
	printf("Current frequency is %lf.\n", frequency);

	if(fabs(frequency-100.0) <= THRESH)
		return 0; // frequency is close enough

	// signal to the AVR we have the data
	digitalWrite(AVR_DIR,HIGH);

	// wait for the AVR_STATUS pin to go high
	while(!digitalRead(AVR_RDY));

	// tell the AVR which direction to move the frequency
	if(frequency > 100.0)
		digitalWrite(AVR_DIR,LOW);
	// no need to change the pin if the frequency is too low

	// wait for the AVR_STATUS pin to go low
	while(digitalRead(AVR_RDY));

	// tell the AVR we don't have any data
	digitalWrite(AVR_DIR,LOW);

	// reset the AVR
	AVR_reset();

	return 1; // frequency needs to get closer
}
