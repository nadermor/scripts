#include <stdio.h>
#include <math.h>

/* 
    gcc sine.c without -lm: 
    gcc -lm sine.c is wrong.
    
    /tmp/cc8spt7g.o: In function `main':
    sine.c:(.text+0x9a): undefined reference to `sin' 
    collect2: error: ld returned 1 exit status
*/

void main() {
    int angle_degree;
    double angle_radian, pi, value;
    printf("Compute a table of the sine function\n\n");

    pi = 4.0 * atan(1.0);
    printf("Value of PI = %f \n", pi);
    printf("Value of M_PI = %f \n\n", M_PI);

    printf("Angle\tSine\n");

    angle_degree = 0;
    while(angle_degree <= 360) {
        angle_radian = pi * angle_degree / 180.0;
        value = sin(angle_radian);

        printf ("%3d\t% f\n", angle_degree, value);
        /* The invisible Pus sign
         * use %+f to add plus sign for non-neg numbers
         * use % f to add invisible plus sign
         */

        angle_degree += 10;
        }
}
