// <stdio.h>  -> defining I/O routines
// <ctype.h>  -> defining character manipulation routines
// <string.h> -> defining string manipulation routines
// <math.h>   -> defining mathematical routines
// <stdlib.h> -> defining number conversion, storage allocation and similar tasks
// <stdarg.h> -> defining libraries to handle routines with variable numbers of arguments
// <time.h>   -> defining time-manipulation routines
// 
// In addition, the following header files exist:
// 
// <assert.h> -> defining diagnostic routines
// <setjmp.h> -> defining non-local function calls
// <signal.h> -> defining signal handlers
// <limits.h> -> defining constants of the int type
// <float.h>  -> defining constants of the float type


/* include headers of library */
/* defined for all routines   */
/* in the file                */
#include <stdio.h>
#include <string.h>
/* prototyping of functions   */
/* to allow type checks by    */
/* the compiler               */

// All identifiers in C need to be declared before they are used. This is true for functions as well as variables. For functions the declaration needs to be before the first call of the function. A full declaration includes the return type and the number and type of the arguments. This is also called the function prototype.
int n_char(char[]); // decalaration
void main() {
    int  n;
    char string[50];
    /* strcpy(a,b) copies string b into a */
    /* defined via the stdio.h header     */
    strcpy(string, "Hello World");

    /* call own function */
    n = n_char(string); 
    printf("Length of string = %d\n", n);   
}

/* definition of local function n_char */
int n_char(char string[]) {
    /* local variable in this function   */
    int n;
    /* strlen(a) returns the length of   */
    /* string a                          */
    /* defined via the string.h header   */
    n = strlen(string);
    if (n > 50) 
        printf("String is longer than 50 characters\n");

    /* return the value of integer n  */
    return n;
}
