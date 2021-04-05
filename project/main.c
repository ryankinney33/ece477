#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "ClassicCon.h"

int main() {
	// make and initialize the controller struct
	WiiClassic con, prev;
	con_init(&con);

	prev = con;

	while(1){
		//con_dump_buttons(&con); // print the current status of the controller
		con_update(&con); // get new button data
		con_print_dig_status(&con,&prev);
		prev = con;

		//printf("\n");
	}
	return 0;
}
