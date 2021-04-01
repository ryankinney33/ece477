#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>

#define SAMPLE_C 50
#define THRESH 0.3

#define WAVE_IN 0

// This function sets pin WAVE_IN as pull-down input
void gpio_init();

// This function measures the frequency of a square wave on pin 0
double get_freq();

int main(){
	gpio_init();

	// get and print info about the frequency
	double frequency = get_freq();
	printf("Frequency = %lf\n",frequency);

	if(fabs(frequency-100.0) <= THRESH)
		printf("Frequency is just about right.\n");
	else if(frequency < 100.0)
		printf("Frequency is too low.\n");
	else
		printf("Frequency is too high.\n");


	exit(0);
}

void gpio_init(){
	wiringPiSetup(); // set up wiring pi

	// set pin 0 as pull-down input
	pinMode(WAVE_IN,INPUT);
	pullUpDnControl(WAVE_IN,PUD_DOWN);
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
	// the average frequency is the number of samples/sum of the times
	return (double)SAMPLE_C/sum;
}
