#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "ClassicCon.h"
#include "CCCons.h"

// Button map for WiiClassic.B array
static const char keyMap[18] = "ABXY-+H^V<>ZLZRLR"; //button layout of con->B

// Initializes the controller
void con_init(WiiClassic* con){
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
	con_update(con);
}

void con_update(WiiClassic* con){
	unsigned char values[6]; // There are 6 1-byte registers with the information for button presses

	// write zero to reset offset for reading bytes
	if(wiringPiI2CWrite(con->fd, 0x00) < 0){
		//printf("Failed to write 0x00\n");
		perror("Error");
	}else{
		// read the junk value to get to required data
//		wiringPiI2CRead(con->fd);


		// reads the 6 bytes for the button data. Stores them in an array
		for(int i = 0; i < 0x6; ++i){
			values[i] = (unsigned char)wiringPiI2CRead(con->fd);
			//printf("%x\n",values[i]); // used for debugging purposes
		}

		//printf("\n"); // used for debugging

		// go through and update all the button statuses according to the array

		// start with A/B/X/Y
		con->B[0] = (values[5]>>BA_5)&1; // A
		con->B[1] = (values[5]>>BB_5)&1; // B
		con->B[2] = (values[5]>>BX_5)&1; // X
		con->B[3] = (values[5]>>BY_5)&1; // Y

		// Minus/plus/home
		con->B[4] = (values[4]>>BM_4)&1; // -
		con->B[5] = (values[4]>>BP_4)&1; // +
		con->B[6] = (values[4]>>BH_4)&1; // H

		// next do D-pad
		con->B[7] = (values[5]>>BDU_5)&1;  // ^
		con->B[8] = (values[4]>>BDD_4)&1;  // V
		con->B[9] = (values[5]>>BDL_5)&1;  // <
		con->B[10] = (values[4]>>BDR_4)&1; // >

		// Trigger/shoulder buttons
		con->B[11] = (values[5]>>BZL_5)&1; // ZL
		con->B[12] = (values[5]>>BZR_5)&1; // ZR
		con->B[13] = (values[4]>>BLT_4)&1; // L
		con->B[14] = (values[4]>>BRT_4)&1; // R

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
}

// Print ALL the buttons of the struct to stdout
void con_dump_buttons(WiiClassic* con){
	// digital inputs
	for(int i = 0, k = 0; i < 15; ++i){ // loop over button array
		char key[] = {keyMap[k++],0,0};
		if(key[0] == 'Z')
			key[1] = keyMap[k++];
		printf("%s = %d\n",key,con->B[i]);
	}

	// analog inputs
	printf("RX = %d\n",con->rx);
	printf("RY = %d\n",con->ry);
	printf("LX = %d\n",con->lx);
	printf("LY = %d\n",con->ly);
	printf("RT = %d\n",con->rt);
	printf("LT = %d\n",con->lt);
}

// Prints if any digital inputs are different
void con_print_dig_status(WiiClassic* current, WiiClassic* previous){
	// The logic for telling if a button is the result of previous - current
	// If 0, no change (either button is not pressed or held)
	// If 1, button pressed
	// If -1, button released
	for(int i = 0, k = 0; i < 15; ++i){
		char state[12];
		// string for what button we are on
		char key[] = {keyMap[k++],0,0};
		if(key[0] == 'Z')
			key[1] = keyMap[k++];

		int status = previous->B[i]-current->B[i];
		if(!status) // no change
			//sprintf(state,"%s",current->B[i]? "not pressed":"held");
			continue;
		else if(status == 1)
			sprintf(state,"%s","pressed");
		else
			sprintf(state,"%s","released");

		printf("%s was %s\n",key,state);
	}
}
