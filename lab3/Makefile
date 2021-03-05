# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab3a lab3b

lab3a: lab3a.o
	$(CC) -o lab3a lab3a.o -lwiringPi

lab3b: lab3b.o
	$(CC) -o lab3b lab3b.o -lwiringPi

%.o: %.c
	$(CC) -c $< -o $@

lab3a.o: lab3a.c
lab3b.o: lab3b.c

clean:
	rm -rf lab3a lab3b *.o
