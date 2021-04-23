#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// used for I2C
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "ClassicCon.h"

// Macros to help extract the button data from the read values
#include "CCCons.h"

// open the i2c device, choose which device to communicate with and return the file descriptor
int i2c_init(char* filepath,int addr){
	// open the I2C device for reading and writing
	int fd = open(filepath, O_RDWR);
	if(fd < 0){ // check for errors
		fprintf(stderr,"Error %i during open: %s\n",errno,strerror(errno));
		exit(1);
	}

	// communicate with the device specified by address
	if(ioctl(fd,I2C_SLAVE, addr) < 0){
		fprintf(stderr,"Error %i communcating with the device at %x: %s\n",errno,addr,strerror(errno));
		close(fd);
		exit(1);
	}

	// done, return the file descriptor
	return fd;
}

// writes some bytes to unencrypt the controller
void unencrypt(int fd){
	// to unencrypt, write 0x55 to 0xF0 then 0x00 to 0xFB
	static unsigned char msg1[] = {0xF0,0x55};
	static unsigned char msg2[] = {0xFB,0x00};

	// write first byte
	if(write(fd, msg1, 2) != 2){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		fprintf(stderr,"Could not write %x to %x.\n",msg1[1],msg1[0]);
		close(fd);
		exit(1);
	}

	// give the controller some time
	usleep(200);

	// write second byte
	if(write(fd, msg2, 2) != 2){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		fprintf(stderr,"Could not write %x to %x.\n",msg2[1],msg2[0]);
		close(fd);
		exit(1);
	}

	// the controller is now unencrypted and ready for use
}

// buf is a pointer to an array of 6 bytes allocated by the caller
void read_controller(int fd, unsigned char* buf){
	// first, write 0x00
	static const unsigned char send[1] = {0x00};
	if(write(fd,send,1) != 1){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		close(fd);
		exit(1);
	}

	// controller needs time
	usleep(200);

	// read 6 bytes into buf
	if(read(fd,buf,6) != 6){
		fprintf(stderr,"Error %i from read: %s\n",errno,strerror(errno));
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
	static unsigned char buf[6] = {0,0,0,0,0,0}

	// read the button data from the controller into buf
	read_controller(con->fd,buf);

	// use bitwise logic to get the data out of buf

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
