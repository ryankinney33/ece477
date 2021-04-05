#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/sleep.h>

void update_clock_speed(void);

int main(){
	update_clock_speed() // adjust OSCCAL



	while(1); // busy loop
}

void update_clock_speed(void){
	// Read oscillator offset sign
	char temp = eeprom_read_byte((void*)1);
	// 0 is positive, 1 is negative
	// erased reads as ff (avoid that)

	if(temp==0 || temp==1){ // dont change if sign is invalid
		if(temp == 0){
			temp = eeprom_read_byte((void*)0);
			if(temp != 0xff) OSCCAL += temp;
		}else{
			temp = eeprom_read_byte((void*)0);
			if(temp != 0xff) OSCCAL -= temp;
		}
	}
}
