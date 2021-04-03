#ifndef _CLASSICCONTROLLER_H_
#define _CLASSICCONTROLLER_H_

#define DEVICE_ID 0x52

typedef struct WiiClassic {
	unsigned int rx:5, ry:5; // Right joystick coordinates
	unsigned int lx:6, ly:6; // Left joystick coordinates

	unsigned int rt:5, lt:5; // analog buttons of LT and RT

	unsigned int a:1,b:1,x:1,y:1; // A, B, X, Y button status
	unsigned int minus:1,plus:1,home:1; // -,+, and home button status
	unsigned int up:1,down:1,left:1,right:1; // D-pad button status
	unsigned int zl:1,zr:1,l:1,r:1; // digital shoulder buttons

	int fd; // the file handler returned from wiringPiI2CSetup

} WiiClassic;

void controller_init(WiiClassic* con); // initializes the controller
void controller_update(WiiClassic* con); // updates the buttons
void controller_print_status(WiiClassic* con); // prints the button status to stdout
#endif
