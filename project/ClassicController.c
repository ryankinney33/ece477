/*
 * Defines the functions to communicate with the Wii Classic Controller
 * Uses i2c for commuincation and curses to show the output.
 *
 * Author: Ryan Kinney
 * ECE 477 - Spring 2021
 * April 24, 2021
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <curses.h>

// used for I2C
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "ClassicController.h"

/******************************************************/
// Macros for bit location offsets to map the registers
// to the variables describing each button

// Right analog stick
#define RX_0 6
#define RX_1 6
#define RX_2 7
#define RY_2 0

// Left analog stick
#define LX_0 0
#define LY_1 0

// Right trigger
#define RT_3 0

// Left trigger
#define LT_2 5
#define LT_3 5

// Minus/Plus/Home buttons
#define BM_4 4
#define BH_4 3
#define BP_4 2

// Trigger/Shoulder buttons
#define BRT_4 1
#define BLT_4 5
#define BZL_5 7
#define BZR_5 2

// A/B/X/Y buttons
#define BB_5 6
#define BY_5 5
#define BA_5 4
#define BX_5 3

// D-pad buttons
#define BDR_4 7
#define BDD_4 6
#define BDL_5 1
#define BDU_5 0
/******************************************************/

static const char keyMap[18] = "ABXY-+H^V<>ZLZRLR"; //button layout of con->B

// open the i2c device, choose which device to communicate with and return the file descriptor
int i2c_init(char* filepath,int addr){
	// open the I2C device for reading and writing
	int fd = open(filepath, O_RDWR);
	if(fd < 0){ // check for errors
		endwin();
		fprintf(stderr,"Error %i during open: %s\n",errno,strerror(errno));
		exit(1);
	}

	// communicate with the device specified by address
	if(ioctl(fd,I2C_SLAVE, addr) < 0){
		endwin();
		fprintf(stderr,"Error %i communcating with the device at %x: %s\n",errno,addr,strerror(errno));
		close(fd);
		exit(1);
	}

	// done, return the file descriptor
	return fd;
}

// buf is an array of size numBytes allocated by the caller
void read_controller(int fd, unsigned char* buf, int numBytes, unsigned char reg){
	// first, write reg
	if(write(fd,&reg,1) != 1){
		endwin();
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		close(fd);
		exit(1);
	}

	// controller needs time
	usleep(200);

	// read numBytes bytes into buf
	if(read(fd,buf,numBytes) != numBytes){
		endwin();
		fprintf(stderr,"Error %i from read: %s\n",errno,strerror(errno));
		close(fd);
		exit(1);
	}
}

// writes some bytes to unencrypt the controller
void unencrypt(int fd){
	// to unencrypt, write 0x55 to 0xF0 then 0x00 to 0xFB
	unsigned char msg1[2] = {0xF0,0x55};
	unsigned char msg2[2] = {0xFB,0x00};

	// write first byte
	if(write(fd, msg1, 2) != 2){
		endwin();
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		close(fd);
		exit(1);
	}

	// give the controller some time
	usleep(2000);

	// write second byte
	if(write(fd, msg2, 2) != 2){
		endwin();
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		close(fd);
		exit(1);
	}

	// the controller is now unencrypted; verify a classic controller was connected
	// read 2 bytes from register 0xFE and make sure 0x0101 is read
	read_controller(fd,msg1,2,0xFE);
	if(msg1[0] != 0x01 || msg1[1] != 0x01){
		endwin();
		fprintf(stderr,"Error: A Wii Classic Controller was not connected.");
		close(fd);
		exit(1);
	}
}


// Initialize the controller structure
void con_init(WiiClassic* con, char* filepath, int addr){
	// first open i2c and start communicating with the device
	con->fd = i2c_init(filepath,addr);

	// next, unencrypt the controller's registers to read the data
	unencrypt(con->fd);

	// finally, read the data from the controller to initialize the
	// the data in the struct
	con_update(con);
}

// Update the button data in the structure
void con_update(WiiClassic* con){
	// buffer for holding the read data
	unsigned char values[6];

	// read the button data from the controller into values
	// the button data is obtained from a 6 byte read of register 0x00
	read_controller(con->fd,values,6,0x00);

	// use bitwise logic to get the data out of values

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
	con->rx |= (values[2]>>RX_2)&0x1;
}

// Print which buttons were just pressed/released
void con_status(WiiClassic* current, WiiClassic* previous){
	static int initColors = 1;

	if(initColors){
		initColors = 0;
		// pressed buttons will be green
		init_pair(1,COLOR_GREEN,COLOR_BLACK);

		// not pressed buttons will be red
		init_pair(2,COLOR_RED,COLOR_BLACK);

		// analog switches will be cyan
		init_pair(3,COLOR_CYAN,COLOR_BLACK);
	}

	// move the cursor back to 0,0
	move(0,0);

	// The logic for telling the state of a button is the result of previous - current
	// If 0, no change (either button is not pressed or held)
	// If 1, button pressed
	// If -1, button released
	for(int i = 0, k = 0; i < 15; ++i){
		char state[12];
		// string for what button we are on
		char key[] = {keyMap[k++],' ',0};
		if(key[0] == 'Z')
			key[1] = keyMap[k++];

		int status = previous->B[i]-current->B[i];
		if(!status) // no change
			sprintf(state,"%s",current->B[i]? "is not pressed":"is held");
		else if(status == 1)
			sprintf(state,"%s","was pressed");
		else
			sprintf(state,"%s","was released");

		// choose the color based on current->B[i]
		attrset(COLOR_PAIR(current->B[i]+1)|A_BOLD);
		// print the string to the buffer
		printw("%s %s\n",key,state);
		attroff(COLOR_PAIR(current->B[i]+1));
		attroff(A_BOLD);
	}

	// next, the analog switches
	attrset(COLOR_PAIR(3) | A_BOLD);
	printw("\nLX = %2d\n",current->lx);
	printw("LY = %2d\n",current->ly);
	printw("LT = %2d\n",current->lt);
	printw("RT = %2d\n",current->rt);
	printw("RY = %2d\n",current->ry);
	printw("RX = %2d\n",current->rx);
	attroff(COLOR_PAIR(3));
	attroff(A_BOLD);
	// flush the buffer, updating the screen
	refresh();
}


// Prints which buttons were just pressed/released to stdout on multiple lines
void con_print_buttons(WiiClassic* current, WiiClassic* previous){

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

/*
 * TODO:
 *  Games and stuff don't really care about printing the status
 *  of the buttons and stuff. Write functions to return the states
 *  of the buttons that another program can use to process the information
 */
