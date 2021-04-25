/*
 * A driver program showing an example of using
 * the Wii Classic Controller interface.
 *
 * Author: Ryan Kinney
 * ECE 477 - Spring 2021
 * April 24, 2021
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>

#include <curses.h>

#include "ClassicController.h"

volatile sig_atomic_t keepRunning = 1; // for gracefully exiting the program
void sigint_handler(int x){
	keepRunning = 0;
}

int main(int argc, char* argv[]){
	// handle SIGINT
	struct sigaction act;
	act.sa_handler = sigint_handler;
	sigaction(SIGINT, &act, NULL);

	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);
	int address = 0x52; // TODO: this should not be hard-coded

	// initialize structures for the current and previous controller values
	WiiClassic con, prev;
	con_init(&con, filename, address);
	prev = con;

	// determine which output mode to use based on arguments
	static int outputMode = 0;
	if(argc > 1) // if ANY arguments were passed, display when buttons were pressed/released
		outputMode = 1;
	// TODO: better command line arguments

	if(!outputMode){
		// create the ncurses output screen
		initscr();
		curs_set(0); // hide the cursor

		// attempt to start colored output
		if(has_colors()){
			if(start_color() != OK){
				endwin();
				fprintf(stderr,"Could not start colors.\n");
				outputMode = 1; // revert to old output method
			}
		}else{
			endwin();
			fprintf(stderr,"Terminal does not support colors\n");
			outputMode = 1; // rever to old output method
		}
	}
	while(keepRunning){
		// first, print the controller's status
		if(!outputMode){
			con_status(&con,&prev);
		}else{
			con_print_buttons(&con,&prev);
		}

		// update the previous
		prev = con;

		// get new input
		con_update(&con);
	}

	// print a newline or close the screen
	if(outputMode)
		printf("\n");
	else
		// execution finished, close the window
		endwin();

	exit(0);
}

// TODO: Use uinput to make the controller act
//       as keyboard for use in games and stuff
