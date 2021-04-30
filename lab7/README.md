# ECE477 Lab 7
The goal of this lab is to determine the type of accuracy
and resolution that can be attained from the internal
temperature sensor of the ATMega88PA, which is cited to have
a sensitivity of "approximately 1mV per degree C" with an
accuracy of +/- 10 degrees C.

Has a program for the AVR to read the internal temperature sensor
once every second and send it to the Pi via USART. The Pi will save
the read temperature values in a file, "tempature.dat" with one floating
point number for the temperature per line.

Some questions to help guide this analysis:
- When the device is at a constant temperature, do the ADC readings stay constant?
- How much variability is there?
- Does the average of 32 values provide a more constant reading? (it should)
- Does the sum of 32 values provide more information than the average, or are the low 5 bits purely random?
- What kind of resolution and accuracy can you attain using the internal temperature sensor and a software calibration method of your choosing?
