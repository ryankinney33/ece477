#include <stdio.h>
#include <limits.h>

int main() {
    printf("The size of char is %d(%X) bytes\n", sizeof(char),sizeof(char));
    printf("The size of short is %d(%X) bytes\n", sizeof(short), sizeof(short));
    printf("The size of int is %d(%X)) bytes\n", sizeof(int), sizeof(int));
    printf("The size of long is %d(%X)) bytes\n", sizeof(long), sizeof(long));
    printf("The size of float is %d(%X)) bytes\n", sizeof(float), sizeof(float));
    printf("The size of double is %d(%X)) bytes\n", sizeof(double), sizeof(double));
    return 0;
}
