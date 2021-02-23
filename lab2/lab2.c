#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	// we want to receive 1 command line argument, so argc should be 2
	if(argc != 2){
		printf("Error: Expected 1 argument, received %d.\n", argc-1);
		printf("Usage is %s number, where number is an integer constant.\n",argv[0]);
		exit(1);
	}
	exit(0);
}
