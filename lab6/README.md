# ECE477
## Part A
Develop a program for the AVR that initializes the USART for 8 data bits, 2 stop bits, 9600 baud communication.
The program will use the stream stdin and stdout when sending to or receiving from the serial connection. The
ADC will also be initialized to read the bandgap voltage. AVcc will be the analog reference and tied to Vcc.
The AVR will run at 8 MHz, using the clock calibration from Lab 5.

After performaing the initialization, the AVR will wait until the string START is received. Then it will estimate
and report the power supply voltage in a string in the form of `The power rail is approximately 3.297712V\n`.

It will continue to measure the voltage and report the value approximately once per second.

A program for the Pi will be written to open the appropriate serial port, set the setting appropriately, send
the string to the AVR and write the received strings to stdout. The numbers at the end of the string will be
saved to a file, `rail_voltages.dat` with one floating point value per line.

## Part B
A program was written for the Pi to act as a dummy serial terminal. It will read data from the serial port and write it
to stdout. It will also read data from stdin and send it to the serial port. The reading and writing 
can happen simulataneously, as reading and writing will not block communication.
