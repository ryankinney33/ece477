#include <stdio.h>
#include <wiringPi.h>
#include "ClassicController.h"

int main() {
	// make and initialize the controller struct
	WiiClassic con;
	controller_init(&con);

//	while(1){
		controller_print_status(&con); // print the current status of the controller
		//printf("\n");
		//printf("%d\n",con.a);
//		delay(2000); // delay 1000 ms
//		controller_update(&con); // get new input from the user
//		printf("\n");
//	}
	return 0;
}
