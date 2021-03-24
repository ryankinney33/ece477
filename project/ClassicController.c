#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

int main() {
	// initialize the gpio pins and setup I2C
	int fd = wiringPiI2CSetup(DEVICE_ID);

	if(fd == -1){
		printf("Failed to initialize I2C communication.\n");
		return 1;
	}

	printf("I2C communication successfully initialized.\n");

	printf("%x\n",wiringPiI2CRead(fd));
	return 0;
}
