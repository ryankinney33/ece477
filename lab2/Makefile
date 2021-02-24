# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab2a lab2b

lab2a: lab2a.o
	$(CC) -o lab2a lab2a.o -lwiringPi

lab2b: lab2b.o
	$(CC) -o lab2b lab2b.o -lwiringPi

%.o: %.cpp
	$(CC) -c $< -o $@

lab2a.o: lab2a.c
lab2b.o: lab2b.c

clean:
	rm -rf lab2a lab2b *.o
