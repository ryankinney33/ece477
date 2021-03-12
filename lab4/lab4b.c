/*
	The purpose of this program is to something cool
	related to part A. Implements a simple game with
	the two buttons and the LED.
	- When the LED is at the rightmost position and
	  button A is pressed, the LED will start moving left.
	- When the LED is at the leftmost position and button
	  B is pressed, the LED will start moving right.
	- When a button is pressed when the LED is moving left
	  or right and is not at the edge, the player who did
	  not press the button gets a point.
	- When the game starts, the LED does not move until
	  button A is pressed, which will start the game.
	- Whenever a point is scored, the LED will be at
	  either the leftmost or rightmost edge (depending
	  on who scored). And will not move until the button
	  corresponding to that edge is pressed. (Call this serving)
	- If the LED is not moving, and the player who is not
	  the server presses the button, nothing happens.
	  If Ctrl+C (SIGINT) is sent to the program, the game
	  will end and the final score will be printed.

	Author: Ryan Kinney
	ECE 477 - March 12, 2021
	March 12, 2021
*/

#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

// Macros for "moving" the LED
#define LEFT  1
#define RIGHT -1
#define SERVE 0

// Macros for the state of the game
#define LSERVE 7
#define RSERVE 0


// used to end the game
static int gameRunning = 1;
void intHandler(int x){// handles SIGINT to end the game
	gameRunning = 0;
}

int main(){
	// used to process the end of the game
	// set up the signal handling stuff
	struct sigaction act;
	act.sa_handler = intHandler;
	sigaction(SIGINT,&act,NULL);

	// create button structs and set up the GPIO pins
	button A,B;
	A.pin = 21;
	A.previous = A.current = 0;
	B.pin = 22;
	B.previous = B.current = 0;
	gpio_init(&A,&B);

	// set up variables for lighting the LEDs
	int direction = SERVE;
	int state = RSERVE;
	int litLED = 0;

	int scoreL = 0; // Left score
	int scoreR = 0; // Right score

	while(gameRunning){
		// check if the LED is off the board
		if(litLED > 7 || litLED < 0){
			// see who failed
			if(direction == LEFT){ // LED moved off the left, left player failed
				// right player scored, let them serve again
				printf("Right scored. Current score: Left: %d, Right: %d\n",scoreL,++scoreR);
				litLED = 0;
				state = RSERVE;
			}else{ // LED moved off the right, the right player failed
				printf("Left scored.  Current score: Left: %d, Right: %d\n",++scoreL,scoreR);
				litLED = 7;
				state = LSERVE;
			}
			// places the LED back on the board, depending on who is serving
			direction = SERVE;
			litLED = state;
		}

		// light the LED
		set_leds(litLED);

		// delay for 256 ms and process user input
		for(int i = 0; i < 256; ++i){
			get_input(&A);
			get_input(&B);

			// process the input
			if(A.current && !A.previous){
				// button A was just pressed
				if(direction == LEFT){
					litLED = -5; // place the LED off the board
					direction = RIGHT; // signify that left has scored
				}else if(direction == SERVE && state == RSERVE){
					// start the game
					direction = LEFT;
				}else if(direction == RIGHT){
					if(litLED == RSERVE){ // the LED was at the right edge
						// successful hit
						direction = LEFT;
					}else{
						// bad hit, move LED off board
						litLED = -5;
					}
				}
			}else if(B.current && !B.previous){
				// button A was just pressed
				if(direction == RIGHT){
					litLED = 10; // place the LED off the board
					direction = LEFT; // signify that left has scored
				}else if(direction == SERVE && state == LSERVE){
					// start the game
					direction = RIGHT;
				}else if(direction == LEFT){
					if(litLED == LSERVE){ // the LED was at the right edge
						// successful hit
						direction = RIGHT;
					}else{
						// bad hit, move LED off board
						litLED = -5;
					}
				}
			}
			delay(1);
		}
		// update the LED, delay and reduce the delay counter
		litLED += direction;
	}
	// the game has ended, print the final score
	printf("\n\nGame over.\nFinal Score:\nLeft:  %d\nRight: %d\n",scoreL,scoreR);

	// shut off the LEDs
	for(int i = 0; i < 7; ++i){
		digitalWrite(i,LOW);
	}
}
