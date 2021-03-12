# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab4a lab4b

lab4a: lab4a.o gpio.o
	$(CC) -o lab4a lab4a.o gpio.o -lwiringPi

lab4b: lab4b.o gpio.o
	$(CC) -o lab4b lab4b.o gpio.o -lwiringPi

%.o: %.c
	$(CC) -c $< -o $@

lab4a.o: lab4a.c gpio.h
lab4b.o: lab4b.c gpio.h
gpio.o:  gpio.c gpio.h

clean:
	rm -rf lab4a lab4b *.o
