# ECE477
## Part A
This part of the lab is an introduction to AVR programming. Basically, install the avr tools, flash the sample
code, and measure the frequency of the square wave.

The AVR programmer is made using the SPI interface on the Pi's GPIO pins. A program will be written to measure
the frequency of the square wave outputted by the AVR. The frequency should be 100Hz and the program will tell
if the frequency was too high, too low, or just right.

Outside of the program mentioned previously, values will be written to the EEPROM of the AVR to calibrate
the oscillator to get the frequency as close to 100Hz as possible.

## Part B
For this part of the lab, the AVR sample code was modified to include a function that will accept a digital input
that tells whether or not the frequency is too high or too low. The value in the EEPROM will then be adjusted accordingly.
The program on the Pi will be adapted to send a digital signal to the AVR to tell it which direction to change
the frequency. Basically, the program will (in a loop until the frequency is close enough to 100Hz) measure the
frequency, set a pin high/low to tell the AVR which direction to change the frequency, and reset the AVR.
