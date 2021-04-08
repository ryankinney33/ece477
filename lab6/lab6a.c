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

float extract_number(char* buf);
void start(int serial_port);
void get_data(int serial_port, FILE* file);

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
	if(data == NULL){
		fprintf(stderr, "Error %i from fopen: %s\n", errno, strerror(errno));
		close(serial);
		exit(1);
	}

	// initialization is done; start doing stuff
	start(serial);

	while(keepRunning){
		get_data(serial,data);
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
		close(port);
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
		close(port);
		exit(1);
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

void start(int serial_port){
	// buffer
	char buf[256] = "Please type \"START\" (no quotes) to start.\n";
	printf("%s",buf);
	do{
		scanf("%s",buf); // get input from stdin
		if(write(serial_port, buf, strlen(buf)) == -1){ // send the input to the AVR, check for errors
			fprintf(stderr, "Error %i from write: %s\n", errno, strerror(errno));
			close(serial_port);
			exit(1);
		}
	}while(strcmp(buf,"START")); // stop getting input once "START" is sent
}



void get_data(int serial_port, FILE* file){
	static char* buf = "\0"; // input buffer

	// read the data from the serial port
	int x = read(serial_port,&buf,sizeof(buf));
	if(x == -1){ // check for error
		fprintf(stderr, "Error %i from read: %s\n", errno, strerror(errno));
		keepRunning = 0; // exit program on error
	}
	else if(x){ // check if anything was actually read
		printf("%s",buf); // print what was received from the AVR to stdout
		fprintf(file,"%lf\n",extract_number(buf)); // save the number in rail_voltage.dat
		sleep(1);
	}
}
