#include "gpio.h"
#include <wiringPi.h>

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
