#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

// Macros for button masks
// Right analog stick
#define RX_0 0x3<<6
#define RX_1 0x3<<6
#define RX_2 0x1<<7
#define RY_2 0x1F

// Left analog stick
#define LX_0 0x3F
#define LY_1 0x3F

// Right trigger
#define RT_3 0x1F

// Left trigger
#define LT_2 0x3<<5
#define LT_3 0x7<<5

// Minus/Plus/Home buttons
#define BM_4 1<<4
#define BH_4     1<<3
#define BP_4  1<<2

// Trigger/Shoulder buttons
#define BRT_4 1<<1
#define BLT_4 1<<5
#define BZL_5 1<<7
#define BZR_5 1<<2

// A/B/X/Y buttons
#define BB_5 1<<6
#define BY_5 1<<5
#define BA_5 1<<4
#define BX_5 1<<3

// D-pad buttons
#define BDR_4 1<<7
#define BDD_4 1<<6
#define BDL_5 1<<1
#define BDU_5 1<<0

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
	if(send_byte(con->fd, 0x55, 0xF0) == -1){ // check for errors
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

	// reads the 6 bytes for the button data. Stores them in an array
	for(int i = 0; i < 6; ++i){
		values[i] = (unsigned char)wiringPiI2CRead(con->fd);
	}

	// go through and update all the button statuses according to the array
	










}
