# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab3a

lab3a: lab3a.o
	$(CC) -o lab3a lab3a.o -lwiringPi

%.o: %.cpp
	$(CC) -c $< -o $@

lab3a.o: lab3a.c

clean:
	rm -rf lab3a lab3b *.o
