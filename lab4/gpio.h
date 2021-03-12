/*
	This file provides function prototypes for
	initializing the GPIO pins, getting input,
	and lighting up the LEDs.

	Author: Ryan Kinney
	ECE 477 - Spring 2021
	March 12, 2021
*/

#ifndef _GPIO_H_
#define _GPIO_H_

// struct for input button status
typedef struct button {
	int previous, current, pin;
} button;

// Sets the pins listed in the button struct as
// input and sets it to use the pull up resistor
// Also sets pins 0-7 as outputs for lighting the LEDs
void gpio_init(button* A, button* B);

// Lights up the LED specified by num.
// For example, if num is 5, LED 5 will be lit up
void set_leds(int num);

// Updates the current and previous fields of the
// button struct.
void get_input(button* s);

#endif
