# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

all: main.out

main.out: main.o ClassicController.o
	$(CC) -o main.out main.o ClassicController.o -lwiringPi

%.o: %.c
	$(CC) -c $< -o $@

main.o: main.c ClassicController.h
ClassicController.o: ClassicController.c ClassicController.h ClassicControllerConstants.h

clean:
	rm -rf *.out *.o
