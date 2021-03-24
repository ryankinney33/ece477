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
	delay(1); // delays an ms
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
	if(send_byte(con->fd, 0x55, 0xF0) == -1){
		printf("Writing 0x55 to 0xF0 failed.\n");
		exit(1);
	}

	if(send_byte(con->fd, 0x00, 0xFB) == -1){
		printf("Writing 0x00 to 0xFB failed.\n");
		exit(1);
	}

	// the controller is now unencrypted, run update_controller to set the button states
	controller_update(con);
}

void controller_update(WiiClassic* con){
	unsigned char values[6]; // There are 6 1-byte registers with the information for button presses

	// write zero to reset offset for reading bytes
	if(wiringPiI2CWrite(con->fd, 0x00) == -1){
		printf("Failed to write 0x00\n");
		exit(1);
	}
	// read the junk value
	wiringPiI2CRead(con->fd);

	for(int i = 0; i < 6; ++i){
		values[i] = (unsigned char)wiringPiI2CRead(con->fd);
		// print the value in value[i]
		printf("Byte %d = %x\n",i,values[i]);
	}
}
