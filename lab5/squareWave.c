#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/sleep.h>
#include <util/delay.h>

void init_pwm(void);
void update_clock_speed(void);

void init_digitalIO(void);
void clock_cal(void);

//This code is distributed with no warranty expressed or implied.
//It does not contain any known bugs, but has not been tested.
//What it is intended to do is use the first two bytes of eeprom
//as an offset and direction for adjusting the internal oscillator
//The first byte is an unsigned byte that is the amount to adjust
//the OSCCAL register.  The next byte will be 0 or 1 depending on
//whether the adjustment should be positive (0) or negative (1).
//The value 0xff is intentionally avoided to distinguish unprogrammed
//eeprom locations.

//main first calls update_clock_speed to make the adjustments to
//the oscillator calibration and then calls init_pwm to set up
//a 100Hz 50% duty cycle square wave on OC1A (pin 15 on the 28 pin
//DIP package).

// it then calls init_gpio to initialize pins D0 and D1 (pins 2 and
// 3 on the 28 pin DIP package) these pins are controlled by the Pi
// these pins will communicate with the Pi to update the EEPROM to
// calibrate the internal oscillator, clock_cal is then called
// to look at the values of the pins and update the eeprom accordingly


int main()
{
  update_clock_speed();  //adjust OSCCAL
  init_pwm();            //set up hardware PWM
  init_digitalIO();      // set up gpio pins to communicate with the Pi

  while(1){
    clock_cal();
  }
}

//read the first two bytes of eeprom, if they have been programmed
//use the first byte as an offset for adjusting OSCCAL and the second as
//the direction to adjust 0=increase, 1=decrease.
//Any offset is allowed and users are cautioned that it is possible to
// adjust the oscillator beyond safe operating bounds.
void update_clock_speed(void)
{
  char temp;
  temp=eeprom_read_byte((void *)1); //read oscillator offset sign
                                    //0 is positive 1 is  negative
                                    //erased reads as ff (so avoid that)
  if(temp==0||temp==1)      //if sign is invalid, don't change oscillator
  {
      if(temp==0)
	  {
	     temp=eeprom_read_byte((void *)0);
		 if(temp != 0xff) OSCCAL+=temp;
	  }
	  else
	  {
	     temp=eeprom_read_byte((void *)0);
		 if(temp!=0xff) OSCCAL -=temp;
	  }
  }
}


void init_pwm(void)
{
  // **************************************************************
  // ***   Timer 1                                                *
  // **************************************************************

  DDRB |= (1<<PB1);  //set OC1A as an output
  OCR1A=19999;    //set initial compare at 50%
  ICR1=39999U; // 8 MHz /40000/2 = PWM frequency = 100 Hz
  TCCR1A = (1<<COM1A1); //zeros in COM1B1,COM1B0,WGM11,WGM10
  //internal clock, no prescaler , PWM mode 8
  TCCR1B = (1<<WGM13) | (1<<CS10);
}

// sets one pin as digital input and one as digital output
void init_digitalIO(void){
	DDRD |= (1<<PD1); // set pin D1 as output
	PORTD &= ~(1<<PD1); // set pin D1 low
	DDRD &= ~(1<<PD0); // set pin D0 as input
}

void clock_cal(void){
	// we want to wait until PD0 is high
	if(PIND&(1<<PD0)){
		// the pin is high, the Pi, has measured the frequency
		PORTD |= (1<<PD1); // set pin D1 high to tell the Pi we are ready

		// delay 1 ms for the Pi to update pin PD0
		_delay_ms(1.0);

		char offS = eeprom_read_byte((void*)0); // offset
		char offD = eeprom_read_byte((void*)1); // direction of offset

		// set the values to zero if the direction bit is "erased"
		if(offD == 0xFF){
			offS = 0;
			offD = 0;
		}else if(offS == 0){
			offD = 0;
		}

		// next, set the values in offS and offD according to the direction
		// If PD0 is high, we need to increase the clock speed
		// If PD0 is low, we need to decrease the clock speed
		if(PIND&(1<<PD0)){
			// increase the clock speed
			if(offD)
				// the current offset direction is negative, decrease the offset
				offS--;
			else
				// offset direction is currently positive, increase the offset
				offS += (offS < 0xFF)? 1:0; // keep offS within 0-0xFF
		}else{
			// decrease the clock speed
			if(offD)
				// the current offset direction is negative, increase the offset
				offS += (offS < 0xFF)? 1:0; // keep offS within 0-0xFF
			else{
				// offset direction is positive
				if(offS){
					offS--; // offset is non-zero, decrease it
				}else{
					// reverse direction and increase the offset
					offD = 1;
					offS++;
				}
			}
		}

		// update the values in EEPROM
		eeprom_update_byte((void*)0,offS);
		// wait until eeprom is finished
		eeprom_busy_wait();

		eeprom_update_byte((void*)1,offD);
		eeprom_busy_wait();

		// signal to the Pi that we are done
		PORTD &= ~(1<<PD1);
	}
}
