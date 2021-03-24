#include <wiringPi.h>
#include "ClassicController.h"

int main() {
	// make and initialize the controller struct
	WiiClassic con;
	controller_init(&con);

	for(int i = 0; i < 10; ++i){
		controller_print_status(&con); // print the current status of the controller
		delay(100); // delay 100 ms
		controller_update(&con); // get new input from the user
	}
	return 0;
}
