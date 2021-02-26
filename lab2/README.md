# ECE 477 Lab 2
## Part A
Write a program that will accept a single command line argument that is an integer
constant recognizable by C, namely it can be a decimal integer, octal (if it has a leading 0),
or hex (with a leading 0x). The constant should be in the range of 0-0xff with each bit
corresponding to an LED. High bits turn on an LED and low bits turn them off.

## Part B
Basically adapts Part A to take multiple command line arguments and cycles through them
on the LEDs with a short delay between each(kinda like a lightshow). Skips over illegal inputs.
