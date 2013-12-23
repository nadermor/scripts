#include <stdio.h>

void main() {
    // char variables[]; (wrong)
    // io_file.c: In function ‘main’:
    // io_file.c:4:10: error: array size missing in ‘variables’
    
    // char variables[] = "string"; (correct)
    // char variables[100]; (correct)
    
    scanf("format string", variables);
    printf("%s",variables);

    // sprintf(string, "format string", variables);
    // scanf(string, "format string", variables);

    // FILE *fp;
    // fp = fopen(name, mode);

    // fscanf(fp, "format string", variable list);
    // fprintf(fp, "format string", variable list);

    // fclose(fp);

}
