#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>

#define SAMPLE_C 50

// This function initializes the GPIO pins
// Pins 26 and 2 as outputs
// Pins 0 and 3 as pull-down inputs
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

	// set pin 0 as pull-down input
	pinMode(0,INPUT);
	pullUpDnControl(0,PUD_DOWN);

	// set pin 26 as output (for resetting the AVR)
	pinMode(26,OUTPUT);
	digitalWrite(26,HIGH);

	// set pin 2 as output (for telling the AVR what direction the clock speed needs to move)
	pinMode(2,OUTPUT);
	digitalWrite(2,LOW);

	// set pin 3 as pull-down input (get status from AVR)
	pinMode(3,INPUT);
	pullUpDnControl(3,PUD_DOWN);
}

// Resets the AVR by sending a low-pulse on pin 26
void AVR_reset(){
	digitalWrite(26,LOW);
	delay(1);
	digitalWrite(26,HIGH);
	delay(100);
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
				currentState = digitalRead(0);
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

	if(fabs(frequency-100.0) <= 0.25)
		return 0; // frequency is close enough

	// signal to the AVR we have the data
	digitalWrite(2,HIGH);

	// wait for pin 3 to go high
	while(!digitalRead(3));

	// tell the AVR which direction to move the frequency
	if(frequency > 100.0)
		digitalWrite(2,LOW);
	// no need to change the pin if the frequency is too low

	// wait for pin 3 to go low
	while(digitalRead(3));

	// tell the AVR we don't have any data
	digitalWrite(2,LOW);

	// reset the AVR
	AVR_reset();

	return 1; // frequency needs to get closer
}
