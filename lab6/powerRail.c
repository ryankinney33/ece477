#define F_CPU 8000000UL // 8 MHz oscillator
#define BAUD 9600UL // 9600 baud rate for USART

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/sleep.h>


void update_clock_speed(void);
void init_usart(void);

int main(){
	update_clock_speed(); // adjust OSCCAL
	init_usart(); // initialize USART0


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

void init_usart(void){
	// ubrr value = fosc/(16*baud rate) - 1
	uint16_t UBRRn = F_CPU/(BAUD<<4)-1; // should be about 51

	// Set the baud rate
	UBRR0H = (uint8_t)(UBRRn>>8);
	UBRR0L = (uint8_t)(UBRRn);

	// Enable the receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	// Set the frame format: 8 data bits, 2 stop bits
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
