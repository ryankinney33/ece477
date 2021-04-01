# Usage:
# make          Compile binaries
# make lab5a	Compile lab5a program
# make lab5b	Compile lab5b program
# make flash	Compile and/or flash the avr program
# make          Delete ALL binaries

CC=avr-gcc

CFLAGS=-Wall -mmcu=atmega88
OBJ2HEX=avr-objcopy

TARGET=squareWave

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

lab5a: lab5a.c
	gcc -o lab5a lab5a.c -lwiringPi -lm

lab5b: lab5b.c
	gcc -o lab5b lab5b.c -lwiringPi -lm

clean:
	rm -rf *.hex *.obj lab5a lab5b
