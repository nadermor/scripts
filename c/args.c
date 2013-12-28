/* args: command-line arguments, or switches */
#include <stdio.h>

void main(int argc, char** argv) {

/* char**:  a pointer to a character array (a character string), can also write this as char* argv[]. 
 * the array argv contains, in order, params (strings are delineated by whitespace, ie. spaces),
 * as well as the command itself. 
 *
 * The integer argc gives the total number of strings, 
 * and is therefore equal to the number of arguments plus one. 
 * For example, if you typed 
        a.out -i 2 -g -x 3 4 
 * argc = 7
 * argv[0] = "a.out"
 * argv[1] = "-i"
 * argv[2] = "2"
 * argv[3] = "-g"
 * argv[4] = "-x"
 * argv[5] = "3"
 * argv[6] = "4"
 *
 * Note that the arguments, even the numeric ones, are all strings at this point. 
 * It is the programmer's job to decode them and decide what to do with them.
 */
    int i;

    printf("argc = %d\n", argc);
    for (i = 0; i < argc; i++)
        // i < argc, not <=
        // deref-ing an array with index out of range yields wierd stuff, very wierd
        printf("argv[%d] = \"%s\"\n", i, argv[i]);
}
