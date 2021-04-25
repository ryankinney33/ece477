# ECE 477 Final Project
A program to interface with the Wii Classic Controller Pro over the i2c protocol.

Many tears were produced over this.

On RPi:
- Run `raspi-config`, go to `Interface Options`, and enable I2C.
- Requires the following packages: `libi2c-dev` and `i2c-tools`.

When running the program, there are two output modes, selected by using command line arguments.
1. For the normal mode, where all the button states and the values from the analog inputs are printed,
do not pass any command line arguments. Call the program like `./main.out`.
2. For the mode that prints when and which button was pressed or released, call the program with
some command line arguments. Any number of command line arguments will work and it does not
matter what is passed. Call the program like `./main.out Never gonna give you up` or
`./main.out Never gonna let you down`, or `./main.out 1`.
