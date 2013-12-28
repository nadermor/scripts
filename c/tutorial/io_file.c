#include <stdio.h>

void main() {
    // char variables[]; (wrong)
    // io_file.c: In function ‘main’:
    // io_file.c:4:10: error: array size missing in ‘variables’
    
    // char variables[] = "string"; (correct)
    // char variables[20] = "string"; (correct)
    // char variables[20]; (correct)
    
    // scanf("%s", &variables);
    // printf("%s", variables);

    // sprintf(string, "format string", variables);
    // scanf(string, "format string", variables);

    /* I/O to/from files */
    /* Syntax:
     * fscanf(fp, "format string", variable list); 
     * fprintf(fp, "format string", variable list);
     */
    int i;

    /* open a file named foo.dat, 
     * write Sample Code + 1-10 */
    FILE *fp; // defined in stdio
    fp = fopen("foo.dat", "w");
    // mode: "r" = read, "w" = write, "a" = append

    fprintf(fp, "Sample Code\n\n");
    for (i = 1; i <= 10; i++)
        fprintf(fp, "i = %d\n", i);
    fclose(fp);
    

}
