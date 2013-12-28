/* some convensions for UNIX programmers: 
 * 1. switches and key terms are always preceded by a "-"' character. 
 This makes them easy to recognize as you loop through the argument list. 
 * 2. depending on the switch, the next arg may be interpreted as int, float, or just kept as character strings. 

 * With these conventions, the most common way to "parse" the argument list is with a for loop and a switch statement
 */

#include <stdio.h>
#include <stdlib.h> // atoi(), atof()
// They convert from character strings to ints and doubles, respectively.

// Usage: a.out -a 3 -b 5.6 -c "I am a string" -d 222 111
// (The use of double quotes with -c here makes sure that the shell treats the entire string, 
// including the spaces, as a single object.)

// note it's the shell that delimits the string with quotes!

void main(int argc, char** argv) {
    /* Set defaults for all parameters: */

    int a_value = 0;
    float b_value = 0.0;
    char* c_value = NULL;
    int d1_value = 0, d2_value = 0;

    int i;

    /* Start at i = 1 to skip the command name. */

    for (i = 1; i < argc; i++) {

        /* Check for a switch (leading "-"). */

        if (argv[i][0] == '-') {

            /* Use the next character to decide what to do. */

            switch (argv[i][1]) {

                case 'a':
                    a_value = atoi(argv[++i]);
                    break;

                case 'b':
                    b_value = atof(argv[++i]);
                    break;

                case 'c': 
                    c_value = argv[++i];
                    break;

                case 'd':
                    d1_value = atoi(argv[++i]);
                    d2_value = atoi(argv[++i]);
                    break;

            }
        }
    }

    printf("a = %d\n", a_value);
    printf("b = %f\n", b_value);
    if (c_value != NULL) printf("c = \"%s\"\n", c_value);
    printf("d1 = %d, d2 = %d\n", d1_value, d2_value);
}


/* this setting is buggy
for example: ./a.out -c -d 1 2
the output would be
c = "-d"
*/
