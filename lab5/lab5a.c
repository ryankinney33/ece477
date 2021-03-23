#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// This function sets pin 0 as pull-down input
void gpio_init();

int main(){
	gpio_init();

	// a counter for how many loop iterations ran
	long long unsigned int counter = 0;

	int riseEdge = 0; // a flag for whether or not a rising edge was found
	int currentState = 1, previousState = 1; // current and previous input states
	// note, these are both initialized to 1 in order to ignore the initial state of the input pin
	// when the program is started (if the input starts high, that will not count as a rising edge

	while(!riseEdge){
		// update the current and previous states
		previousState = currentState;
		currentState = digitalRead(0);
		riseEdge = currentState && !previousState; // check for rising edge
		counter++;
	}

	// rising edge found, print results
	printf("Rising Edge detected!\n");
	printf("Counter = %llu\n",counter);

	exit(0);
}

void gpio_init(){
	wiringPiSetup(); // set up wiring pi

	// set pin 0 as pull-down input
	pinMode(0,INPUT);
	pullUpDnControl(0,PUD_DOWN);

}
