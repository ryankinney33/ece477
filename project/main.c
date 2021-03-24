#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

#include "ClassicController.h"

int main() {

	WiiClassic con;

	controller_init(&con);

	printf("%x\n",wiringPiI2CRead(con.fd));
	return 0;
}
