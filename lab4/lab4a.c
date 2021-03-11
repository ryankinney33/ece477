#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LEFT 1
#define RIGHT -1


// struct for input button status
typedef struct button {
	int previous, current, pin;
} button;


void gpio_init(button* A, button* B);
void set_leds(int num);
void get_input(button* s);

int main(){
	// create button structs and set up the GPIO pins
	button A,B;
	A.pin = 21;
	A.previous = A.current = 0;
	B.pin = 22;
	B.previous = B.current = 0;
	gpio_init(&A,&B);

	// set up variables for lighting the LEDs
	int direction = LEFT;
	int LEDstate = 0;
	int delayNum = 1024;

	while(1){
		// set the LEDs
		set_leds(LEDstate);

		int check_delay = 0;

		// wait the delay
		for(int i = 0; i < delayNum; ++i){
			// get input from the user
			get_input(&A);
			if(A.current && !A.previous && check_delay > 0){ // button A was just pressed
				// Decrease delay
				delayNum >>= 1;
				if(delayNum < 32){ // Reverse the direction if necessary
					delayNum = 32;
					direction = -direction;
				}
				check_delay = 20;
			}else{
				get_input(&B);
				if(B.current && !B.previous){ // button B was just pressed
					// Increase delay
					delayNum <<= 1;
					if(delayNum > 1024){ // reverse direction if necessary
						delayNum = 1024;
						direction = -direction;
					}
				}
				check_delay = 20;
			}
			delay(1);
			--check_delay;
		}

		// update the LED according to the direction
		LEDstate += direction;

		if(LEDstate < 0 || LEDstate > 7){
			// place the LED back in range
			LEDstate = (direction==LEFT)? 0:7;
		}
	}

	exit(0);
}

void gpio_init(button* A, button* B){
	wiringPiSetup();

	for(int i = 0; i < 8; ++i){
		pinMode(i,OUTPUT);
	}

	// inputs on the pins in the structs
	pinMode(A->pin,INPUT);
	pinMode(B->pin,INPUT);

	// use internal pull down resistors
	pullUpDnControl(A->pin,PUD_UP);
	pullUpDnControl(B->pin,PUD_UP);
}

void set_leds(int num){
	for(int i = 0; i < 8; ++i){
		digitalWrite(i,LOW);
	}
	digitalWrite(num,HIGH);
}

// save the previous input and gets the new input
void get_input(button* s){
	s->previous = s->current;
	s->current = !digitalRead(s->pin);
}
