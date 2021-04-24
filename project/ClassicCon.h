#ifndef _CLASSIC_CON_H_
#define _CLASSIC_CON_H_

// A struct that holds all the necessary information to use
// use the controller. This struct is initialized by calling
// con_init() with the device file path and 7-bit address
// of the controller (which is most likely 0x52)
typedef struct WiiClassic{
	unsigned int rx:5, ry:5; // Right joystick coordinates
	unsigned int lx:6, ly:6; // Left joystick coordinates

	unsigned int rt:5, lt:5; // Analog switches of LT and RT

	unsigned char B[15]; // {A,B,X,Y,-,+,H,^,V,<,>,ZL,ZR,L,R}

	int fd; // the file descriptor for the i2c device
} WiiClassic;


// Initializes the controller for use
// Opens the device specified by filepath
// Opens communication with the device at addr
// Unencrypts the controller then reads the initial button values
void con_init(WiiClassic* con, char* filepath, int addr);

// Reads the data from the controller and updates
// the passed WiiClassic struct
void con_update(WiiClassic* con);

// Dumps the raw data for each button and analog input to stdout
void con_dump_data(WiiClassic* con);

// Prints which buttons were just pressed/released to stdout on multiple lines
void con_button_status(WiiClassic* current, WiiClassic* previous);

// Prints the values of the analog switches on one line
void con_analog(WiiClassic* con);

// Prints the values of all the buttons and analog switches on one line and updates on button press
void con_status(WiiClassic* con);
#endif
