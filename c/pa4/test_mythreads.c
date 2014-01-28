/*	User-level thread system
 *
 */

#include <setjmp.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h"

static int MyInitThreadsCalled = 0;	/* 1 if MyInitThreads called, else 0 */
static int head = 0; /* next available thread slot */
static int tail = 0; /* curr thread running */
static int emptyslot = 0;

static struct thread {			/* thread table */
	int valid;			/* 1 if entry is valid, else 0 */
	jmp_buf env;			/* current context */
    int next;
    int prev;
} thread[MAXTHREADS];

#define STACKSIZE	65536		/* maximum size of thread stack */

/*	MyInitThreads () initializes the thread package.  Must be the first
 *	function called by any user program that uses the thread package.
 */

void MyInitThreads ()
{
	int i;

	if (MyInitThreadsCalled) {                /* run only once */
		Printf ("InitThreads: should be called only once\n");
		Exit ();
	}

	for (i = 0; i < MAXTHREADS; i++) {	/* initialize thread table */
		thread[i].valid = 0;
        thread[i].next = thread[i].prev = -1;
	}

	thread[0].valid = 1;			/* initialize thread 0 */

	MyInitThreadsCalled = 1;
}

/*	MySpawnThread (func, param) spawns a new thread to execute
 *	func (param), where func is a function with no return value and
 *	param is an integer parameter.  The new thread does not begin
 *	executing until another thread yields to it.
 */

int MySpawnThread (func, param)
	void (*func)();		/* function to be executed */
	int param;		/* integer parameter */
{
	if (! MyInitThreadsCalled) {
		Printf ("SpawnThread: Must call InitThreads first\n");
		Exit ();
	}

    int old_emptyslot = emptyslot; // record original next value in head of queue

    // get next thread slot
    do {
        emptyslot = (emptyslot + 1) % MAXTHREADS;
        if (old_emptyslot == emptyslot) return (-1); // next remain unchanged if full
    } while (thread[emptyslot].valid); 

	if (setjmp (thread[(emptyslot-1) % MAXTHREADS].env) == 0) {	/* save context of thread 0 */

		/* The new thread will need stack space.  Here we use the
		 * following trick: the new thread simply uses the current
		 * stack, and so there is no need to allocate space.  However,
		 * to ensure that thread 0's stack may grow and (hopefully)
		 * not bump into thread 1's stack, the top of the stack is
		 * effectively extended automatically by declaring a local
		 * variable (a large "dummy" array).  This array is never
		 * actually used; to prevent an optimizing compiler from
		 * removing it, it should be referenced.
		 */

		char s[STACKSIZE];	/* reserve space for thread 0's stack */
		void (*f)() = func;	/* f saves func on top of stack */
		int p = param;		/* p saves param on top of stack */
        // Printf("p's address: %d",(int)&p);

		if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
			Printf ("Stack space reservation failed\n");
			Exit ();
		}

		if (setjmp (thread[emptyslot].env) == 0) {	/* save context of 1 */
			longjmp (thread[head].env, 1);	/* back to thread 0 */
		} // here, it's sort of like a link between old and new

		/* here when thread 1 is scheduled for the first time */

		(*f) (p);			/* execute func (param) */

		MyExitThread ();		/* thread 1 is done - exit */
	}

	thread[emptyslot].valid = 1;	/* mark the entry for the new thread valid */
    thread[emptyslot].prev = tail;
    tail = emptyslot;

	return emptyslot;		/* done spawning, return new thread ID */
}

/*	MyYieldThread (t) causes the running thread to yield to thread t.
 *	Returns ID of thread that yielded to t (i.e., the thread that called
 *	MyYieldThread), or -1 if t is an invalid ID.
 */

int MyYieldThread (t)
	int t;				/* thread being yielded to */
{
	if (! MyInitThreadsCalled) {
		Printf ("YieldThread: Must call InitThreads first\n");
		Exit ();
	}

	if (t < 0 || t >= MAXTHREADS) {
		Printf ("YieldThread: %d is not a valid thread ID\n", t);
		return (-1);
	}
	if (! thread[t].valid) {
		Printf ("YieldThread: Thread %d does not exist\n", t);
		return (-1);
	}

    if (setjmp (thread[head].env) == 0) {
        int tempnext = thread[t].next;
        int tempprev = thread[t].prev;
        int temphead = head;
        int temp2head = thread[t].next;
        int temptail = tail;
        thread[tempnext].prev = tempprev;
        thread[tempprev].next = tempnext;
        thread[temphead].prev = temptail;
        thread[temptail].next = temphead;
        thread[temp2head].prev = t;
        thread[t].next = temp2head;
        tail = head;
        head = t;
        longjmp (thread[t].env, 1);
    }
        return head;
}

/*	MyGetThread () returns ID of currently running thread.
 */

int MyGetThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("GetThread: Must call InitThreads first\n");
		Exit ();
	}
    return head;
}

/*	MySchedThread () causes the running thread to simply give up the
 *	CPU and allow another thread to be scheduled.  Selecting which
 *	thread to run is determined here.  Note that the same thread may
 * 	be chosen (as will be the case if there are no other threads).
 */

void MySchedThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("SchedThread: Must call InitThreads first\n");
        Exit ();
    }

thread[head].prev = tail;
thread[tail].next = head;
tail = head;
head = thread[head].next;

}

/*	MyExitThread () causes the currently running thread to exit.
 */


void MyExitThread ()
{
	if (! MyInitThreadsCalled) {
		Printf ("ExitThread: Must call InitThreads first\n");
		Exit ();
	}
    thread[head].valid = 0;
    head = thread[head].next;
    MySchedThread ();
}
