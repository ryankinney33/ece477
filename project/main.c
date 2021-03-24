#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

int main() {

	WiiClassic con;

	controller_init(&con);

	return 0;
}
