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
float get_railVoltage(void);

// used for using stdin and stdout with USART
FILE usart0_str = FDEV_SETUP_STREAM(USART0_Transmit, USART0_Receive, _FDEV_SETUP_RW);

int main(){
	char buf[100] = "maybe next time";

	update_clock_speed(); // adjust OSCCAL

	USART0_init(); // initialize USART0
	init_ADC(); // initialize ADC0
	_delay_ms(1000); // let serial do its thing

	// wait to receive "START" on USART0
	while(strcmp("START",buf)){
		scanf("%s",buf);
	}

	while(1){ // busy loop; Pi controls reset
		// get the data and send it to the Pi
		printf("The power rail is approximately %lfV\n",get_railVoltage());
		_delay_ms(1000);
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
	// Select AVCC as reference
	// Connect bandgap voltage to the ADC
	// Right-adjust the ADC data
	ADMUX = (1<<REFS0) | (0xE<<MUX0);

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

float get_railVoltage(void){
	// finds the reference voltage based on the ADC value
	// 1.1*/Vref = ADC
	return( (1.1f*(float)0x3FF)/(float)get_ADC());
}
