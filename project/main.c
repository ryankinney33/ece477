#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>

#include "ClassicCon.h"

volatile sig_atomic_t keepRunning = 1; // for gracefully exiting the program
void sigint_handler(int x){
	keepRunning = 0;
}

int main(){
	// handle SIGINT
	struct sigaction act;
	act.sa_handler = sigint_handler;
	sigaction(SIGINT, &act, NULL);

	// TODO: command line arguments
	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);

	int address = 0x52; // TODO: this should not be hard-coded

	// initialize structures for the current and previous controller values
	WiiClassic con, prev;
	con_init(&con, filename, address);
	prev = con;

	while(keepRunning){
		// first, print the controller's status
		con_status(&con);
		// con_analog(&con);

		// update the previous
		prev = con;

		// update the controller
		con_update(&con);
	}
	printf("\n");
	exit(0);
}
