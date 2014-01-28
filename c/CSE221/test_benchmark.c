#define LOOP_COUNT 10000000
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "cycle.h"

ticks tm;
void __attribute__ ((noinline)) func_syscall(void) { getppid(); __asm__ __volatile__(""); }
void __attribute__ ((noinline)) func_donothing(void) { tm = getticks();/* __asm__ __volatile__(""); */}

int main(int argc, char* argv[]) {

	float avg1 = 0.1, avg2 = 0.0, avg3 = 0.0, avg4 = 0.0;
	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
	ticks t1,t2,t3,t4,t5,t6,t7,t8;
	int i;

	for (i = 0; i < LOOP_COUNT; ++i)
	{
		t1 = getticks();
		func_syscall();
		t2 = getticks();
		double d1 = elapsed(t2, t1);
		// printf("func call d1 = %llu\n", t2 - t1);
		// printf("func call d1 = %llu, d2 = %llu\n", d1, d2);
		sum1 += d1;
		//sum2 += (int)d2;
	}

	for (i = 0; i < LOOP_COUNT; ++i)
	{
		t3 = getticks();
		func_donothing();
		t4 = getticks();
		double d2 = elapsed(t4, t3);
		sum2 += d2;
	}

	for (i = 0; i < LOOP_COUNT; ++i)
	{
		t5 = getticks();
		getppid();
		t6 = getticks();
		double d3 = elapsed(t6, t5);
		sum3 += d3;
	}

	for (i = 0; i < LOOP_COUNT; ++i)
	{
		t7 = getticks();
		t8 = getticks();
		double d4 = elapsed(t8, t7);
		sum4 += d4;
	}

	avg1 = (double) sum1 / LOOP_COUNT;
	printf("avg noinline procedual call with system call = %f\n", avg1);
	avg2 = (double) sum2 / LOOP_COUNT;
	printf("avg noinline procedual call = %f\n", avg2);
	avg3 = (double) sum3 / LOOP_COUNT;
	printf("avg inline pure system call = %f\n", avg3);
	avg4 = (double) sum4 / LOOP_COUNT;
	printf("avg inline pure ticks = %f\n", avg4);
	return 0;
}
