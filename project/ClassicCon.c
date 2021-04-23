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
		exit(1);
	}

	// give the controller some time
	usleep(200);

	// write second byte
	if(write(fd, msg2, 2) != 2){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		fprintf(stderr,"Could not write %x to %x.\n",msg2[1],msg2[0]);
		exit(1);
	}

	// the controller is now unencrypted and ready for use
}

// buf is a pointer to an array of 6 bytes allocated by the caller
int read_controller(int fd, unsigned char* buf){
	// first, write 0x00
	static const unsigned char send[1] = {0x00};
	if(write(fd,send,1) != 1){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		return -1;
	}

	// controller needs time
	usleep(200);

	// read 6 bytes into buf
	if(read(fd,buf,6) != 6){
		fprintf(stderr,"Error %i from read: %s\n",errno,strerror(errno));
		return -1;
	}
}
