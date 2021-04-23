#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// used for I2C
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// open the i2c device, choose which device to communicate with and return the file descriptor
int i2c_init(char* filepath,int addr);

// unencrypt the controller
void unencrypt(int fd);

// buf is a pointer to an array of 6 bytes allocated by the caller
void read_controller(int fd, unsigned char* buf);

int main(){
	// TODO: command line arguments
	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);

	unsigned char buf[6]; // holds the 6 bytes from the controller

	// initialize the communication
	int address = 0x52; // TODO: this should not be hard-coded
	int fd = i2c_init(filename,address);

	// unencrypt the controller
	unencrypt(fd);


	int flag = 1;
	while(flag){
		read_controller(fd,buf);

		for(int i = 0; i < 6; ++i){
			flag = flag && buf[i]==good[i];
		}
		if(flag)
			printf("PASSED!\n");
		else
			printf("FAILED!\n");
	}


	exit(0);
}

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

// writes some bytes to decrypt the controller
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
}

// buf is a pointer to an array of 6 bytes allocated by the caller
void read_controller(int fd, unsigned char* buf){
	// first, write 0x00
	static const unsigned char send[1] = {0x00};
	if(write(fd,send,1) != 1){
		fprintf(stderr,"Error %i from write: %s\n",errno,strerror(errno));
		return;
	}

	// controller needs time
	usleep(200);

	// read 6 bytes into buf
	if(read(fd,buf,6) != 6)
		fprintf(stderr,"Error %i from read: %s\n",errno,strerror(errno));
}
