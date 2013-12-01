/* 
 * io word count
 */

#include <stdio.h>

/* getchar() reads one character at a time from stdin,
 * putchar() writes one character at a time to stdout.
 */

void main(void) {
    /* Textbook version:
     *
     * int i, nc;

     * nc = 0;
     * i = getchar();
     * while (i != EOF) {
     *     nc = nc + 1;
     *     i = getchar();
     * }
     * printf("Number of characters in file = %d\n", nc);
     */

    int c; // getchar() actually returns int
    int nc = 0, nl = 0, nw = 0;
    // while (1) {
    //     c = getchar();
    //     if ( c == EOF ) 
    //         break;
    while ((c = getchar()) != EOF) { // mind the parath's here. made a boo boo
        // printf("%c\n", c);
        nc++;
        if (c == '\n') nl++;
        if (c == ' ') nw++;
    }

    printf("This is EOF: %d\n", EOF);
    printf("Number of characters in file = %d\n", nc);
    printf("Number of lines in file = %d\n", nl);
    printf("Number of words in file = %d\n", nw+1);
}
