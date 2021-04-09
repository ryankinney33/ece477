#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <wiringPi.h>

#define DEVICE "/dev/ttyS0"
#define BAUDRATE B9600

#define RESET 26

int gpio_init();
int AVR_reset();
int init_serial();

void read_write(int src, int dst);

static pid_t pid; // global for use in intHandle
static volatile int keepRunning = 1;

// handles SIGINT to exit the program gracefully
void intHandle(int x){
	keepRunning = 0;
	if(!pid){ // child process
		_exit(0);
	}
}


int main(int argc, char* argv[]){
	// used for gracefully exiting the program
	signal(SIGINT, intHandle);
	int src, dst;

	// first, initialize the reset line
	gpio_init();

	AVR_reset(); // reset the AVR
	// initialize the serial port
	int serial = init_serial();

	if(!(pid = fork())){
		// child process, read from stdin, write to serial
		src = 0;
		dst = serial;
	}else if(pid > 0){
		// parent process, read from serial, write to stdout
		src = serial;
		dst = 1;
	}else{
		// fork error
		if(pid < 0){
			fprintf(stderr, "Error %i from fork: %s\n", errno, strerror(errno));
			exit(1);
		}
	}

	// initialization is done; start doing stuff
	while(keepRunning){
		read_write(src,dst);
	}
	wait(NULL); // wait for child to finish

	// done; close port and reset the AVR
	AVR_reset();
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

void read_write(int src, int dst){
	char c; // character for reading and writing
	int x = read(src,&c,1); // read from src
	if(x == -1){ // check for error
		fprintf(stderr, "Error %i from read: %s\n", errno, strerror(errno));
		exit(1);
	}
	else if(x && keepRunning){
		if(write(dst,&c,1) == -1){
			fprintf(stderr, "Error %i from write: %s\n", errno, strerror(errno));
			exit(1);
		}
	}
}
