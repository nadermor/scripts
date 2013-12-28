#include <stdio.h>

void main() {
    float x, y;
    float *fp, *fp2;

    x = 6.5;

    printf("Value of x: %f, address of x: %p\n", x, &x);

    fp = &x;

    printf("Value in memory location fp: %f\n", *fp);

    *fp = 9.2;
    printf("New value of *fp: %f\n", *fp);
    printf("New value of x: %f\n", x);

    *fp += 1.5;
    printf("Final value of x: %f\n", x);

    y = *fp;
    fp2 = fp;
    *fp2 -= 1.5;
    printf("Transferred value into y: %f and *fp2: %f\n", y, *fp2);
}
