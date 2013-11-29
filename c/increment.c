#include <stdio.h>

void main() {
    int x = 10;

    x = (x++);
    printf("x: %d\n", x);
    x = (++x);
    printf("x: %d\n", x);
    x = (x++);
    printf("x: %d\n", x);
    x = (++x);
    printf("x: %d\n", x);

    x = 10;
    x = (x++) + (++x) + (x++) + (++x);
    printf("x: %d\n", x);
}
