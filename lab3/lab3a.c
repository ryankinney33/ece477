#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	FILE *fp;
	fp = fopen("/proc/loadavg","r");

	if(fp == NULL){
		perror("Error");
		exit(-1);
	}

	double loadAVG = 0.0;
	fscanf(fp,"%lf",&loadAVG);

	printf("%lf\n",loadAVG);

	fclose(fp);
	exit(0);
}
