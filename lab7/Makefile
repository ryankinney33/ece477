# Usage:
# make          Compile binaries
# make lab5a	Compile lab5a program
# make lab5b	Compile lab5b program
# make flash	Compile and/or flash the avr program
# make          Delete ALL binaries

CC=avr-gcc

CFLAGS=-Wall -mmcu=atmega88 -u vfprintf -lprintf_flt -lm -Os
OBJ2HEX=avr-objcopy

TARGET=temperature

ADFLAGS=-C /home/pi/avrdude_gpio.conf -c pi_1 -p m88p

.PHONY: all flash fuses eeprom clean

all: $(TARGET).obj lab7S terminal

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

lab7S: lab7S.c
	gcc -o lab7S lab7S.c -lwiringPi

terminal: terminal.c
	gcc -o terminal terminal.c -lwiringPi

clean:
	rm -rf *.hex *.obj lab7S terminal
