#include <stdio.h>
#include <limits.h>

int main() {
	// The first part of Part A of the lab asks to print the sizes of various variable types
	printf("The size of char is %d bytes\n", sizeof(char),sizeof(char));
	printf("The size of short is %d bytes\n", sizeof(short), sizeof(short));
	printf("The size of int is %d bytes\n", sizeof(int), sizeof(int));
	printf("The size of long is %d bytes\n", sizeof(long), sizeof(long));
	printf("The size of float is %d bytes\n", sizeof(float), sizeof(float));
	printf("The size of double is %d bytes\n", sizeof(double), sizeof(double));

	// Part A also asks to print various values defined in the <limits.h> header
	// in decimal and in hex
	printf("INT_MAX is %d in decimal or %X in hex\n", INT_MAX, INT_MAX);
	printf("UINT_MAX is %u in decimal or %X in hex\n", UINT_MAX, UINT_MAX);
	printf("LONG_MAX is %d in decimal or %X in hex\n", LONG_MAX, LONG_MAX);
	printf("ULONG_MAX is %u in decimal or %X in hex\n", ULONG_MAX, ULONG_MAX);
	return 0;
}
