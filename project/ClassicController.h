#ifndef _CLASSICCONTROLLER_H_
#define _CLASSICCONTROLLER_H_

#define DEVICE_ID 0x52

typedef struct WiiClassic {
	unsigned char rx, ry; // Right joystick coordinates
	unsigned char lx, ly; // Left joystick coordinates

	unsigned char rt, lt; // digital buttons of LT and RT

	unsigned char a,b,x,y; // A, B, X, Y button status
	unsigned char minus,plus,home; // -,+, and home button status
	unsigned char up,down,left,right; // D-pad button status

	int fd; // the file handler returned from wiringPiI2CSetup

} WiiClassic;

void controller_init(WiiClassic* con); // initializes the controller
void controller_update(WiiClassic* con); // updates the buttons

#endif
