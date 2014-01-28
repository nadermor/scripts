#include <stdio.h>
#include <math.h>

/* 
    gcc sine.c -lm (correct)
    gcc -lm sine.c (wrong)

    gcc sine.c without -lm: 
    
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
    // M_PI is defined in <math.h>

    printf("Angle\tSine\n");

    angle_degree = 0;
    while(angle_degree <= 360) {
        angle_radian = pi * angle_degree / 180.0;
        value = sin(angle_radian);

        /* %.nd     integer (optional n = number of columns; if 0, pad with zeroes)
         *          i.e. set n cols first, then print from right to left
         *
         * %m.nf    float or double (optional m = number of columns, n = number of decimal places)
         *          note: m includes decimal point
         *
         * %ns      string (optional n = number of columns)
         * %c       character
         * \n \t    new line / tab
         * \g       ring the bell ("beep") on the terminal (might be deprecated)
         */

        printf ("%3d\t% f\n", angle_degree, value);
        /* The invisible Pus sign
         * use %+f to add plus sign for non-neg numbers
         * use % f to add invisible plus sign
         */

        angle_degree += 10;
        }
}
