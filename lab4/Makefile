# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: lab4a lab4b

lab4a: lab4a.o
	$(CC) -o lab4a lab4a.o -lwiringPi

lab4b: lab4b.o
	$(CC) -o lab4b lab4b.o -lwiringPi

%.o: %.c
	$(CC) -c $< -o $@

lab4a.o: lab4a.c
lab4b.o: lab4b.c

clean:
	rm -rf lab4a lab4b *.o
