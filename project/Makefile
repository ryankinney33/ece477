# Usage:
# make          Compile binaries
# make          Delete ALL binaries

CC=gcc

OBJS=main.o ClassicController.o

all: main.out

main.out: $(OBJS)
	$(CC) -o main.out $(OBJS) -lcurses

%.o: %.c
	$(CC) -c $< -O2 -o $@

main.o: main.c ClassicController.h
ClassicController.o: ClassicController.c ClassicController.h

clean:
	rm -rf *.out *.o
