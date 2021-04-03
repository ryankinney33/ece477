# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

OBJS=main.o ClassicCon.o

all: main.out

main.out: $(OBJS)
	$(CC) -o main.out $(OBJS) -lwiringPi

%.o: %.c
	$(CC) -c $< -o $@

main.o: main.c ClassicCon.h
ClassicCon.o: ClassicCon.c ClassicCon.h CCCons.h

clean:
	rm -rf *.out *.o
