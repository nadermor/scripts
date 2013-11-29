#include <stdio.h>

#define SIZE 3

void main() {
    float x[SIZE];
    float* fp;
    int i;

    for (i = 0; i < SIZE; i++)
        x[i] = 0.5 * (float)i;

    printf("Printing x[i]: \n");

    for (i = 0; i < SIZE; i++)
        printf("   %d   %f\n", i, x[i]);

    fp = x;

    printf("Printing *(fp+i): \n");
    for (i = 0; i < SIZE; i++)
        printf("   %d   %f\n", i, *(fp+i));
}
