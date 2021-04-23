#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ClassicCon.h"

int main(){
	// TODO: command line arguments
	int adapter_num = 1; // TODO: this should not be hard-coded
	char filename[20];
	snprintf(filename,19, "/dev/i2c-%d", adapter_num);

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
