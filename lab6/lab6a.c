#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <wiringPi.h>


#define RESET 26

#define BAUDRATE B9600
#define DEVICE "/dev/ttyS0"

int gpio_init();
int AVR_reset();
int init_serial();
//void get_data(char* buf, FILE* file
float extract_number(char* buf);

static volatile int keepRunning = 1;

// handles SIGINT to exit the program gracefully
void intHandle(int x){
	keepRunning = 0;
}

int main(){
	// used for gracefully exiting the program
	signal(SIGINT, intHandle);

	// first, initialize the reset line
	gpio_init();

	// initialize the serial port
	int serial = init_serial();
	AVR_reset();
	delay(1300); // wait a bit for the AVR to do its thing

	// opens/creates a file for writing data
	FILE* data = fopen("rail_voltages.dat","w");

	// buffer
	char buf[256] = "\0";
	do{
		scanf("%s",buf); // get input from stdin
		write(serial, buf, strlen(buf)); // send the input to the AVR
	}while(strcmp(buf,"START")); // stop getting input once "START" is sent

	while(keepRunning){
		sprintf(buf,"testing testing 123.4567\n");
		printf("%s",buf);
		fprintf(data,"%lf\n",extract_number(buf));
		sleep(1);
	}

	// done; close the files
	fclose(data);
	close(serial);
	printf("\n");
	exit(0);
}

// Set up the pin used to reset the AVR
int gpio_init(){
	wiringPiSetup();

	pinMode(RESET,OUTPUT);
	digitalWrite(RESET,HIGH);
}

// Send a low pulse to the AVR reset line to reset it
int AVR_reset(){
	digitalWrite(RESET,LOW);
	delay(1);
	digitalWrite(RESET,HIGH);
	delay(100); // wait 100 ms for the AVR to get a steady state
}

int init_serial(){
	// open the serial port and check for errors
	int port = open(DEVICE, O_RDWR);
	if(port < 0){
		fprintf(stderr, "Error %i from open: %s\n", errno, strerror(errno));
		exit(1);
	}

	// configure the port with the termios struct
	struct termios tty;
	if(tcgetattr(port, &tty) != 0){ // error checking
		fprintf(stderr, "Error %i from tcgettattr: %s\n", errno, strerror(errno));
		exit(1);
	}

	tty.c_cflag = CS8 | CREAD | CLOCAL | CSTOPB; // 8 bit chars, enable receiver, no modem status lines
	tty.c_iflag = IGNPAR; // ignore parity errors
	tty.c_oflag = 0; // disable all these flags
	tty.c_lflag = 0; // disable all these flags

	tty.c_cc[VTIME] = 50; // wait 5 seconds (50 deciseconds)
	tty.c_cc[VMIN] = 42; // wait for 42 characters from serial

	// set the baud rate
	cfsetispeed(&tty, BAUDRATE);
	cfsetospeed(&tty, BAUDRATE);

	// set the attributes and check for error
	if(tcsetattr(port, TCSANOW, &tty) != 0){
		fprintf(stderr, "Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}

	return port; // return the file descriptor for the port
}

float extract_number(char* buf){
	static const char delim[7] = " \t\r\n\v\f"; // whitespace characters
	char* token;
	char* ptr;

	// get the first token
	token = strtok(buf,delim);

	// go through all the tokens
	while(token != NULL){
		// check if the current token is a number
		if(isdigit(token[0])){
			return strtof(token,&ptr);
		}

		// get next token
		token = strtok(NULL,delim);
	}
	return nanf(""); // returns NaN if no number was found
}
