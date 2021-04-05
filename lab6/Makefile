# Usage:
# make          Compile binaries
# make lab5a	Compile lab5a program
# make lab5b	Compile lab5b program
# make flash	Compile and/or flash the avr program
# make          Delete ALL binaries

CC=avr-gcc

CFLAGS=-Wall -mmcu=atmega88
OBJ2HEX=avr-objcopy

TARGET=powerRail

ADFLAGS=-C /home/pi/avrdude_gpio.conf -c pi_1 -p m88p

.PHONY: all flash fuses eeprom clean

all: lab5a lab5b $(TARGET).obj

flash: $(TARGET).hex
	sudo avrdude $(ADFLAGS) -U flash:w:$(TARGET).hex:i

eeprom:
	sudo avrdude $(ADFLAGS) -F -U eeprom:w:$(TARGET).eeprom:i

%.hex: %.obj
	$(OBJ2HEX) -j .text -j .data -O ihex $< $@

%.obj: %.c
	$(CC) $(CFLAGS) $< -o $@

fuses:
	sudo avrdude $(ADFLAGS) -U lfuse:w:0xe2:m -U hfuse:w:0xd7:m -U efuse:w:0xf9:m

lab6a: lab6a.c
	gcc -o lab6a lab6a.c -lwiringPi

lab6b: lab6b.c
	gcc -o lab6b lab6b.c -lwiringPi

clean:
	rm -rf *.hex *.obj lab6a lab6b
