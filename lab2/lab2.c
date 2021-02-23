#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	// we want to receive 1 command line argument, so argc should be 2
	if(argc != 2){
		printf("Error: Expected 1 argument, received %d.\n", argc-1);
		printf("Usage is %s number, where number is an integer constant.\n",argv[0]);
		exit(1);
	}

	// next, we want to extract the integer value into an int
	// program accepts decimal, octal, and hex
	int number;
	sscanf(argv[1],"%d",&number);
	printf("Received %d, which is 0x%x, which is 0%o.\n",number,number,number);
	exit(0);
}
