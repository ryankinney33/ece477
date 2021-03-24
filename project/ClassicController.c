#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"
#include "ClassicControllerConstants.h"

// sends a byte to the specified location
int send_byte(int fd, unsigned char data, unsigned char location){
	// Bytes are written by first transmitting the location and then the data
	int x = wiringPiI2CWrite(fd, (int)location);
	if(x != -1){
		x = wiringPiI2CWrite(fd, (int)data);
	}
	//	delay(1); // delays 1 ms (may not be needed)
	return x; // x is the error status. If x is -1, then there was an error
}

// Initializes the controller
void controller_init(WiiClassic* con){
	con->fd = wiringPiI2CSetup(DEVICE_ID);
	if(con->fd == -1){
		printf("Failed to initialize I2C communication.\n");
		exit(1);
	}

	// unencrypt the registers
	// The devices are unencrypted by writing 0x55 to 0xF0 and then 0x00 to 0xFB
	if(wiringPiI2CWriteReg8(con->fd, 0xF0, 0x55) == -1){
		printf("Writing 0x55 to 0xF0 failed.\n");
		exit(1);
	}

	if(wiringPiI2CWriteReg8(con->fd,0xFB, 0x00) == -1){
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
	// read the junk value to get to required data
	wiringPiI2CRead(con->fd);

	// reads the 6 bytes for the button data. Stores them in an array
	for(int i = 0; i < 0x6; ++i){
		values[i] = (unsigned char)wiringPiI2CRead(con->fd);
		printf("%x\n",values[i]);
	}

	printf("\n");

	// go through and update all the button statuses according to the array

	// start with A/B/X/Y
	con->a = (values[5]>>BA_5)&1;
	con->b = (values[5]>>BB_5)&1;
	con->x = (values[5]>>BX_5)&1;
	con->y = (values[5]>>BY_5)&1;

	// next do D-pad
	con->up = (values[5]>>BDU_5)&1;
	con->left = (values[5]>>BDL_5)&1;
	con->down = (values[4]>>BDD_4)&1;
	con->right = (values[4]>>BDR_4)&1;

	// Minus/plus/home
	con->minus = (values[4]>>BM_4)&1;
	con->plus = (values[4]>>BP_4)&1;
	con->home = (values[4]>>BH_4)&1;

	// Trigger/shoulder buttons
	con->r = (values[4]>>BRT_4)&1;
	con->l = (values[4]>>BLT_4)&1;
	con->zr = (values[5]>>BZR_5)&1;
	con->zl = (values[5]>>BZL_5)&1;

	// Analog triggers next
	con->rt = (values[3]>>RT_3)&0x1F;
	con->lt = (values[2]&(0x3<<LT_2)) >> 2; // 2 = LT_2-3
	con->lt |= (values[3]>>LT_3)&0x7;

	// Analog joysticks next
	con->lx = (values[0]>>LX_0)&0x3F;
	con->ly = (values[1]>>LY_1)&0x3F;
	con->ry = (values[2]>>RY_2)&0x1F;
	con->rx = ((values[0]>>RX_0)&0x3)<<3; // need 3 open bits
	con->rx |= ((values[1]>>RX_1)&0x3)<<1; // need 1 open bit
	con->rx |= (values[3]>>RX_2)&0x1;

}

// Print ALL the buttons of the struct to stdout
void controller_print_status(WiiClassic* con){
	printf("a = %d\n",con->a);
	printf("b = %d\n",con->b);
	printf("x = %d\n",con->x);
	printf("y = %d\n",con->y);
	printf("- = %d\n",con->minus);
	printf("+ = %d\n",con->plus);
	printf("h = %d\n",con->home);
	printf("^ = %d\n",con->up);
	printf("V = %d\n",con->down);
	printf("< = %d\n",con->left);
	printf("> = %d\n",con->right);
	printf("l = %d\n",con->l);
	printf("r = %d\n",con->r);
	printf("zl = %d\n",con->zl);
	printf("zr = %d\n",con->zr);
	printf("rx = %d\n",con->rx);
	printf("ry = %d\n",con->ry);
	printf("lx = %d\n",con->lx);
	printf("ly = %d\n",con->ly);
	printf("rt = %d\n",con->rt);
	printf("lt = %d\n",con->lt);
}
