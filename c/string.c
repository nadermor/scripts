#include <stdio.h>

/* C does not provide any operator which manipulate entire strings at once.
 * Strings are manipulated either via pointers or via special routines,
 * available from the standard string library string.h. 
 */

void main() {
    char text_1[100], text_2[100], text_3[100];
    char *ta, *tb;
    int i;

    char message[] = "Hello, I am a string.";

    printf("Original message: %s\n", message);
    // %s means iterately deref a pointer to a char array

    i = 0;
    while ( (text_1[i] = message[i]) != '\0')
        i++;

    printf("Text_1: %s\n", text_1);

    ta = message;
    tb = text_2;
    while ( (*tb++ = *ta++) != '\0')
        ;
    printf("Text_2: %s\n", text_2);
}
