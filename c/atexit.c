#include <stdio.h>
#include <stdlib.h>

void testfunction1(void) {
	puts ("message function 1");
}

void testfunction2(void) {
	puts ("message function 2");
}

int main() {
	atexit(testfunction1);
	atexit(testfunction2);
	puts ("message from main function");
	return 0;
}

/* int atexit(void (*func) (void));*/
/* Call a function at exit of the program*/
/* If the program is terminated normally then the function pointed by the func
 *  pointer argument is called.*/
/* If there are more then one atexit functions (multiple calls) then they are
 * all executed in reverse order as the where stacked. ( up to 32 funcs)
 * (Which means that the last function specified is first to execute at exit
 * of the program.)*/
/* Note: one single function can be executed more then once on exit.*/
