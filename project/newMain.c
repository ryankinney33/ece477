#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


// used for I2C
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <i2c/smbus.h>

int main(){
	// TODO: command line arguments
	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);

	// open the I2C device for reading and writing
	int file = open(filename, O_RDWR);
	if(file < 0){ // check for errors
		fprintf(stderr,"Error %i during open: %s\n",errno,strerror(errno));
		exit(1);
	}

	// communicate with the device specified by address
	int address = 0x52; // TODO: this should not be hard-coded
	if(ioctl(file,I2C_SLAVE, address) < 0){
		fprintf(stderr,"Error %i communcating with the device at %x: %s\n",errno,address,strerror(errno));
		close(file);
		exit(1);
	}

	
}
