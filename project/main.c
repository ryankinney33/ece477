#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ClassicCon.h"

int main(){
	// TODO: command line arguments
	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);

	int address = 0x52; // TODO: this should not be hard-coded

	// initialize structures for the current and previous controller values
	WiiClassic con, prev;
	con_init(&con, filename, address);
	prev = con;

	while(1){
		// first, print the controller's status
		// con_status(&con,&prev);
		con_analog(&con);

		// update the previous
		prev = con;

		// update the controller
		con_update(&con);
	}
	exit(0);
}
