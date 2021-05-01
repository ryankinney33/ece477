/*

	The purpose of this program is to read
	the on-board temperature sensor.

	Sends that data over USART once per second
	after receiving "START" via USART, Uses stdin
	and stdout streams for interfacing with the USART.

	Author: Ryan Kinney
	ECE 477 - Spring 2021
	April 27, 2021

*/




#define F_CPU 8000000UL // 8 MHz oscillator
#define BAUD 9600UL // 9600 baud rate for USART

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

void update_clock_speed(void);

void USART0_init(void);
int USART0_Transmit(char byte, FILE* stream);
int USART0_Receive(FILE* stream);

void init_ADC(void);
int16_t get_ADC(void);
float get_temperature(void);

// used for using stdin and stdout with USART
FILE usart0_str = FDEV_SETUP_STREAM(USART0_Transmit, USART0_Receive, _FDEV_SETUP_RW);

int main(){
	char buf[100] = "Please type \"START\" to start.\n";

	update_clock_speed(); // adjust OSCCAL

	USART0_init(); // initialize USART0
	init_ADC(); // initialize ADC0
	_delay_ms(1000); // let serial do its thing

	// print some information for the user
	printf("%s",buf);

	// wait to receive "START" on USART0
	while(strncmp("START",buf,5)){
		fgets(buf,100,stdin);
	}

	while(1){ // busy loop; Pi controls reset
		// get the temperature and print it to stdout
		printf("%lfC\n",get_temperature());
		_delay_ms(1000); // a short delay
	}
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
	uint16_t UBRRn = (F_CPU/(BAUD<<4))-1; // should be about 51

	// Set the baud rate
	UBRR0H = (uint8_t)(UBRRn>>8);
	UBRR0L = (uint8_t)(UBRRn);

	UCSR0A = 0;

	// Enable the receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	// Set the frame format: 8 data bits, 2 stop bits
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	// setup stdin and stdout streams
	stdin = &usart0_str;
	stdout = &usart0_str;
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

void init_ADC(void){
	// Select 1.1V bandgap voltage as reference
	// Connect the temperature sensor (ADC8) to the ADC
	// Right-adjust the ADC data
	ADMUX = (3<<REFS0) | (8<<MUX0);

	// enable the ADC and set the prescaler
	// choose the prescaler so that the clock is between 50KHz and 200KHz
	ADCSRA = (1<<ADEN)|(3<<ADPS1); // 64 is chosen, 8MHz/64 = 125kHz
	ADCSRB = 0;
	DIDR0 = 0; // disbable the digital inputs
}

int16_t get_ADC(void){
	// start the ADC conversion
	ADCSRA |= (1<<ADSC);

	// wait for the ADC conversion to complete
	while(ADCSRA&(1<<ADSC));

	return ADC;
}

float get_temperature(void){
	// finds the temperature based on the ADC value
	const static float slope = 0.889837f/32.0f; // slope from equation/32 to adjust for averaging
	// get 32 ADC values
	int32_t sum = 0;
	for(int i = 0; i < 32; ++i){
		sum += get_ADC();
	}

	// next, convert the ADC to the temperature and return it
	return((float)sum*slope-284.911f);
}
