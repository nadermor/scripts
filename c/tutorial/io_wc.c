/* 
 * io word count, use pipe to run the program
 * eg. cat text | ./a.out
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
        // printf("%d\n", sizeof(c)); // 4
        nc++;
        if (c == '\n') nl++;
        if (c == ' ') nw++;
    }

    // This demonstrates c would be EOF after the loop
    // and its value is -1
    printf("This is the last c: %c\n", c); // print ascii -1
    printf("This is the last c+66: %c\n", c+66); // print ascii 'A'
    //printf("%d\n", sizeof(c)); // 4, EOF is also 4 bytes
     
    printf("\n");
    printf("This is EOF in %%d: %d\n", EOF); // %% to print %
    printf("Number of characters in file = %d\n", nc);
    printf("Number of lines in file = %d\n", nl);
    printf("Number of words in file = %d\n", nw+1);
}
