#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

// sends a byte to the specified location
int send_byte(int fd, unsigned char data, unsigned char location){
	// Bytes are written by first transmitting the location and then the data
	int x = wiringPiI2CWrite(fd, (int)location);
	if(x != -1){
		x = wiringPiI2CWrite(fd, (int)data);
	}
	delay(5); // delays a few ms
	return x; // x is the error status. If x is -1, then there was an error
}

// Initializes the controller
void controller_init(WiiClassic* con){
	con->fd = wiringPiI2CSetup(DEVICE_ID);
	if(con->fd == -1){
		printf("Failed to initialize I2C communication.\n");
		exit(1);
	}
	printf("I2C communication successfully initialized.\n");

	// unencrypt the registers
	// The devices are unencrypted by writing 0x55 to 0xF0 and then 0x00 to 0xFB
	int x = send_byte(con->fd, 0x55, 0xF0);
	if(x == -1){
		printf("Writing 0x55 to 0xF0 failed.\n");
		exit(1);
	}

	x = send_byte(con->fd, 0x00, 0xFB);
	if(x == -1){
		printf("Writing 0x00 to 0xFB failed.\n");
		exit(1);
	}

	// the controller is now unencrypted, run update_controller to set the button states
	// controller_update(con);
}
