#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

// Initializes the controller
void controller_init(WiiClassic* con){
	con->fd = wiringPiI2CSetup(DEVICE_ID);
	if(con->fd == -1){
		printf("Failed to initialize I2C communication.\n");
		exit(1);
	}
	printf("I2C communication successfully initialized.\n");
}
