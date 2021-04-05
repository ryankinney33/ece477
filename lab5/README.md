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
For this part of the lab, the Raspberry Pi program has been modified to call avrdude to change the offset
stored in the EEPROM of the AVR to calibrate the clock. It calculates the frequency like in part A, adjusts the offset,
flashes the EEPROM, and resets the AVR. This is repeated until the measured frequency crosses 100Hz.
