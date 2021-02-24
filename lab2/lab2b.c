#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

void LED_setup(){
	/* Pins used:
		LED 0: WiringPi pin 0, BCM GPIO17, Physical pin 17
		LED 1: WiringPi pin 1, BCM GPIO18, Physical pin 12
		LED 2: WiringPi pin 2, BCM GPIO27, Physical pin 13
		LED 3: WiringPi pin 3, BCM GPIO22, Physical pin 15
		LED 4: WiringPi pin 4, BCM GPIO23, Physical pin 16
		LED 5: WiringPi pin 5, BCM GPIO24, Physical pin 18
		LED 6: WiringPi pin 6, BCM GPIO25, Physical pin 22
		LED 7: WiringPi pin 7, BCM GPIO4,  Physical pin 7
	*/
	for(int i = 0; i < 8; ++i){
		pinMode(i,OUTPUT);
	}
}

void keypad_setup(){
	/* Pins used
		C1-Input:  WiringPi pin 21
		C2-Input:  WiringPi pin 22
		C3-Input:  WiringPi pin 23
		C4-Input:  WiringPi pin 24
		R1-Output: WiringPi pin 25
		R2-Output: WiringPi pin 26
		R3-Output: WiringPi pin 27
		R4-Output: WiringPi pin 28
	*/
	// first the inputs (the columns)
	for(int i = 21; i <= 24; ++i){
		pinMode(i,INPUT);
	}

	// then the outputs (the rows)
	for(int i = 25; i <= 28; ++i){
		pinMode(i,OUTPUT);
	}
}

// read the 4 input pins of the keypad, saving them as the 4 least significant bits of an int
int get_col_input(){
	int input = 0;
	for(int i = 24; i >= 21; --i){
		// shift input a 1 to the left and set the last bit according to input
		input = (input<<1) | digitalRead(i);
	}
	return input;
}

int keypad_scan(){
	int key_map[4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}};

	int row, col, colPressed = 5, input=0;
	int key = 0xFF;

	// first set the keypad outputs to 0
	for(int i = 25; i <= 28; ++i){
		digitalWrite(i,LOW);
	}

	// short delay
	delay(5);

	// read the inputs
	input = get_col_input();

	if(input == 0xF) // all the columns are high, no button is pressed
		return key;

	// identify the column of the key pressed (the low bit in input)
	for(col = 0; col < 4 && colPressed == 5; ++col){
		if(!(input & (1<<col)))
			colPressed = col;
	}

	// identify the row of the pressed key
	for(row = 0; row < 4 && key == 0xFF; ++row){
		// set all but the current row to LOW
		for(int i = 25; i <= 28; ++i){
			digitalWrite(i,LOW);
		}
		digitalWrite(row+25, HIGH);

		// short delay
		delay(20);

		// read the column inputs
		input = get_col_input();

		// check if the identified column is low
		if(!(input & (1<<colPressed))){
			// column is low, the key has been identified
			key = key_map[row][colPressed];
		}
	}

	// wait for all the columns to go high (button to be released)
	while(get_col_input() != 0xF);

	return key;
}

int main(int argc, char* argv[]){
	// setup the required GPIO pins
	wiringPiSetup();
	LED_setup();
	keypad_setup();

	return 0;
}
