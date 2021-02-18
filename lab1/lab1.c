/*

The purpose of this program is to print the sizes
of various data types and constants defined in
the header file <limits.h>

Author: Ryan Kinney
ECE 477 - Spring 2021
February 18, 2021

*/


#include <stdio.h>
#include <limits.h>

int main() {
	printf("Part A:\n");

	// The first part of Part A of the lab asks to print the sizes of various variable types
	printf("The size of char is %d bytes\n",sizeof(char));
	printf("The size of short is %d bytes\n", sizeof(short));
	printf("The size of int is %d bytes\n", sizeof(int));
	printf("The size of long is %d bytes\n", sizeof(long));
	printf("The size of float is %d bytes\n", sizeof(float));
	printf("The size of double is %d bytes\n", sizeof(double));

	// Part A also asks to print various values defined in the <limits.h> header
	// in decimal and in hex
	printf("INT_MAX is %d in decimal or %X in hex\n", INT_MAX, INT_MAX);
	printf("UINT_MAX is %u in decimal or %X in hex\n", UINT_MAX, UINT_MAX);
	printf("LONG_MAX is %d in decimal or %X in hex\n", LONG_MAX, LONG_MAX);
	printf("ULONG_MAX is %u in decimal or %X in hex\n\n", ULONG_MAX, ULONG_MAX);

	// Part B asks for something interesting, cool, and related to Part A
	// prints the size of some other data types and more values defined in <limits.h>
	printf("Part B:\n");

	printf("The size of long long is %d bytes\n", sizeof(long long));
	printf("The size of int* is %d bytes\n", sizeof(int*)); // I thought pointers were cool so....
	printf("The size of char* is %d bytes\n", sizeof(char*)); // yeah.......
	printf("SHRT_MAX is %d in decimal or %X in hex\n", SHRT_MAX, SHRT_MAX);
	printf("LLONG_MAX is %lld in decimal or %llX in hex\n", LLONG_MAX, LLONG_MAX);
	printf("ULLONG_MAX is %llu in decimal or %llX in hex\n", ULLONG_MAX, ULLONG_MAX);
	return 0;
}
