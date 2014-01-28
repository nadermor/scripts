#include <stdio.h>

void exchange(int, int);

void main() {
    int a, b;

    a = 5;
    b = 7;
    printf("From main: a = %d, b = %d\n", a, b);

    exchange(a, b);
    printf("Back in main: a = %d, b = %d\n", a, b);
}

void exchange(int a, int b) {
    int temp;

    temp = a;
    a = b;
    b = temp;

    printf("From function exchange(): a = %d, b = %d\n", a, b);
}


