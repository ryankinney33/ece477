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
	if(!sscanf(argv[1],"0%o",&number)){
		// sscanf failed to read an octal number, check for hex
		if(!sscanf(argv[1],"0x%x",&number)){
			// sscanf failed to read a hexadecimal number, check for decimal
			if(!sscanf(argv[1],"%d",&number)){
				// something that was not a number was entered
				printf("Error: Illegal input.\n");
				printf("What was entered: %s.\nExpected a number like 255, 0xFF, or 0377.\n",argv[1]);
				exit(2);
			}
		}
	}

	printf("Received %d, which is 0x%X, which is 0%o.\n",number,number,number);
	exit(0);
}
