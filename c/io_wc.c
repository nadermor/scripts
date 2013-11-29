/* 
 * io word count
 */

#include <stdio.h>

/* getchar() reads one character at a time from stdin,
 * putchar() writes one character at a time to stdout.
 */

void main() {
    /* int i, nc;

     * nc = 0;
     * i = getchar();
     * while (i != EOF) {
     *     nc = nc + 1;
     *     i = getchar();
     * }
     * printf("Number of characters in file = %d\n", nc);
     */

    char c;
    int nc = 0, nl = 0, nw = 0;
    while ((c = getchar() != EOF)) {
        printf("%d\n",c);
        if (c == '\n') nl++;
        if (c == ' ') nw++;
        nc++;
    }

    printf("Number of characters in file = %d\n", nc);
    printf("Number of lines in file = %d\n", nl);
    printf("Number of words in file = %d\n", nw+1);
}
