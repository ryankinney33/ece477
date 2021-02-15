# Usage:
# make		Compile binaries
# make		Delete ALL binaries

CC=gcc

all: lab1.out

lab1.out: lab1.o
	$(CC) -o lab1.out lab1.o

%.o: %.cpp
	$(CC) -c $< -o $@

lab1.o: lab1.c

clean:
	rm -rf *.out *.o
