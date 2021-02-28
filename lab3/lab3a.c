#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

void led_setup(){
	wiringPiSetup();

	for(int i = 0; i < 8; ++i)
		pinMode(i, OUTPUT);
}

int main(){
	// program set up
	led_setup();
	FILE *fp;

	while(1){
		// open the loadavg file and check for error
		fp = fopen("/proc/loadavg","r");
		if(fp == NULL){
			perror("Error");
			exit(-1);
		}
		// read the loadAVG and find the position of the most significant bit
		double loadAVG;
		fscanf(fp,"%lf",&loadAVG);

		// since we want 4.0 in the loadavg to light up 8 LEDs
		// multiply loadAVG by 64 so that 4.0 maps to more than 0xFF
		int temp = ((int)(loadAVG*64.0))>>1;
		int numLEDs = 0;

		while(temp > 0){
			temp >>= 1;
			++numLEDs;
		}

		// numLEDs should be the number of LEDs to light up
		int i;
		for(i = 0; i < numLEDs && i < 8; ++i){ // light up LED 0-numLEDs
			digitalWrite(i,HIGH);
		}
		for(i = numLEDs; i < 8; ++i){ // turn off LED numLEDs-7
			digitalWrite(i,LOW);
		}

		// close the file (for reopening)
		fclose(fp);
		sleep(3);
	}

	exit(0);
}
