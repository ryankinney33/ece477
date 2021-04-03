#include <stdio.h>
#include <wiringPi.h>
#include "ClassicCon.h"

int main() {
	// make and initialize the controller struct
	WiiClassic con, prev;
	con_init(&con);

	prev = con;

	while(con_update(&con) != -1){
		//con_dump_buttons(&con); // print the current status of the controller
//		if(con_update(&con)== -1) // get new button data
//			con_init(&con);
		delay(32);
		con_print_dig_status(&con,&prev);
		prev = con;
		printf("\n");
	}
	return 0;
}
