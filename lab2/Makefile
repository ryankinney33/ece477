# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab2

lab2: lab2.o
	$(CC) -o lab2 lab2.o -lwiringPi

%.o: %.cpp
	$(CC) -c $< -o $@

lab1.o: lab1.c

clean:
	rm -rf lab2 *.o
