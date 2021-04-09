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

void start(int port);
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

	AVR_reset(); // reset the AVR
	delay(1300); // sleep for 1.3 seconds (give the AVR some time)
	// initialize the serial port
	int serial = init_serial();

	// opens/creates a file for writing data
	FILE* data = fopen("rail_voltages.dat","w");
	if(data == NULL){
		fprintf(stderr, "Error %i from fopen: %s\n", errno, strerror(errno));
		close(serial);
		exit(1);
	}

	// initialization is done; start doing stuff
	start(serial);
	char buf[100];

	while(keepRunning){
		get_data(serial,data);
	}

	// done; close the files and reset the AVR
	AVR_reset();
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

	tty.c_cflag = CS8 | CREAD | CLOCAL | CSTOPB; // 8 data bits, 2 stop bits enable receiver, no modem status lines

	tty.c_iflag = IGNPAR; // ignore parity errors
	tty.c_oflag = 0; // disable all these flags
	tty.c_lflag = 0; // canonical processing

	// set the baud rate
	cfsetispeed(&tty, BAUDRATE);
	cfsetospeed(&tty, BAUDRATE);

	// set the attributes and check for error
	if(tcsetattr(port, TCSANOW, &tty) != 0){
		fprintf(stderr, "Error %i from tcsetattr: %s\n", errno, strerror(errno));
		exit(1);
	}

	return port; // return the file descriptor for the port
}

void start(int port){
	// buffer
	char buf[100] = "Please type \"START\" (no quotes) to start.\n";
	printf("%s",buf);
	do{
		fgets(buf,100,stdin); // get input from stdin
		if(write(port,buf,strlen(buf)) == -1){
			fprintf(stderr, "Error %i during write: %s\n", errno, strerror(errno));
			exit(1);
		}
	}while(strncmp(buf,"START",5)); // stop getting input once the first 5 characters of buf are 'START'
}

void get_data(int serial_port, FILE* file){
	static char buf[100] = "\0"; // input buffer

	// read the data from the serial port
	int x = read(serial_port,buf,sizeof(buf));
	if(x == -1){ // check for error
		fprintf(stderr, "Error %i from read: %s\n", errno, strerror(errno));
		keepRunning = 0; // exit program on error
	}
	else if(x && keepRunning){ // check if anything was actually read
		printf("%s",buf); // print what was received from the AVR to stdout
		// iterate through the buffer, writing numbers, periods, and newlines
		for(int i = 0; i < strlen(buf); ++i){
			if(isdigit(buf[i]) || buf[i] == '.'){
				fputc(buf[i],file); // number or decimal point, write it
			}else if(buf[i] == 'V'){
				fputc('\n',file); // end of number, add newline
			}
		}
	}
}
