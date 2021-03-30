#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>

#define SAMPLE_C 50

// This function sets pin 0 as pull-down input
// also sets pin 26 as digital output (for resetting the AVR)
void gpio_init();

int main(){
	gpio_init();

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
	// the average frequency is the number of samples/sum of the times
	printf("%lf\n", (double)SAMPLE_C/sum);
	exit(0);
}

void gpio_init(){
	wiringPiSetup(); // set up wiring pi

	// set pin 0 as pull-down input
	pinMode(0,INPUT);
	pullUpDnControl(0,PUD_DOWN);
}
