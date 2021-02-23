#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	// print the args to stdout
	for(int i = 0; i < argc; ++i){
		printf("argv[%d] is %s\n",i,argv[i]);
	}

	return 0;
}
