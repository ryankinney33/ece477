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
void USART0_init(void);
int USART0_Transmit(char byte, FILE* stream);
int USART0_Receive(FILE* stream);

// used for using stdin and stdout with USART
FILE usart0_str = FDEV_SETUP_STREAM(USART0_Transmit, USART0_Receive, _FDEV_SETUP_RW);

int main(){
	update_clock_speed(); // adjust OSCCAL
	USART0_init(); // initialize USART0
	stdin = stdout = &usart0_str;


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

void USART0_init(void){
	// ubrr value = fosc/(16*baud rate) - 1
	uint16_t UBRRn = F_CPU/(BAUD<<4)-1; // should be about 51

	// Set the baud rate
	UBRR0H = (uint8_t)(UBRRn>>8);
	UBRR0L = (uint8_t)(UBRRn);

	// Enable the receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	// Set the frame format: 8 data bits, 2 stop bits
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	// setup stdin and stdout streams
	stdin = stdout = &usart0_str;
}

int USART0_Transmit(char byte, FILE* stream){
	// send newline characters as \r\n
	if(byte == '\n')
		USART0_Transmit('\r',stream);

	// wait for the transmit buffer to empty
	while(!(UCSR0A&(1<<UDRE0)));

	// put the data into the buffer, sending it
	UDR0 = byte;
	return 0;
}

int USART0_Receive(FILE* stream){
	// wait for data to be recevied
	while(!(UCSR0A&(1<<RXC0)));

	return UDR0; // return the data from the buffer

}
