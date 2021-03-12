#ifndef _GPIO_H_
#define _GPIO_H_

// struct for input button status
typedef struct button {
	int previous, current, pin;
} button;

void gpio_init(button* A, button* B);
void set_leds(int num);
void get_input(button* s);

#endif
