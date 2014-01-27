#include <stdio.h>
#include "cycle.h"

ticks tm1, tm2;

void  func(void) { __asm__ __volatile__ (""); }
void  __inline__ func_inline(void) { __asm__ __volatile__(""); }


int main(int argc, char *argv[])
{

	float avg1 = 0.1, avg2 = 0.0;
	int sum1 = 0;
	int sum2 = 0;
	int i;

	for (i = 0; i < 10000; ++i)
	{
		ticks t1 = getticks();
		func();
		ticks t2 = getticks();
		ticks d1 = t2 - t1;
		// printf("func call d1 = %llu\n", t2 - t1);
		// printf("func call d1 = %llu, d2 = %llu\n", d1, d2);
		sum1 += (int)d1;
		//sum2 += (int)d2;
	}
	avg1 = sum1 / 10000;
	// avg2 = sum2 / 100;
	printf("avg d1 = %f\n", avg1);


	sum1 = sum2 = 0;
	for (i = 0; i < 10000; ++i)
	{
		ticks t1 = getticks();
		// func_inline();
		ticks t2 = getticks();
		ticks d2 = t2 - t1;
		// printf("func_inline call d2 = %llu\n", t2 - t1);
		// sum1 += (int)d1;
		sum2 += (int)d2;
	}
	avg2 = sum2 / 10000;
	printf("avg d2 = %f\n", avg2);
	// printf("avg d1 = %f, avg d2 = %f\n", avg1, avg2);
	// printf("avg2 = %f\n", avg);

	return 0;
}
