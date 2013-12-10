/*      User-level thread system
 *
 */

#include <setjmp.h>
#include <string.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h"

static int MyInitThreadsCalled = 0;     /* 1 if MyInitThreads called, else 0 */

static struct thread {                  /* thread table */
    int valid;                              /* 1 if entry is valid, else 0 */
    jmp_buf env;                    /* current context */
    jmp_buf cleanEnv;               /* clean context */
    void (*f)();
    int p;
} thread[MAXTHREADS];

// scheduling queue
static struct queue {
    int items[MAXTHREADS];
    int front;
    int end;
} queue;

static int currentThread;
static int lastSpawned;
static int spawnedThreads;
static int allSpawned;
static int yielding;
static int exiting;

#define STACKSIZE       65536           /* maximum size of thread stack */

/*      MyInitThreads () initializes the thread package.  Must be the first
 *      function called by any user program that uses the thread package.
 */

int stack_partition (int number_of_partitions) {
    //stack has been partitioned, return
    if (number_of_partitions <= 1) {
        return -1;
        //logic for 1 additional partition to stack
    } else {
        //push SP up by declaring dummy array
        char s[STACKSIZE];
        //check if space got reserved
        if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
            Printf ("Stack space reservation failed\n");
            Exit ();
        }
        //save new environment
        if (setjmp(thread[MAXTHREADS - number_of_partitions + 1].env) == 0) {
            //save the original environment into cleanEnv
            memcpy(thread[MAXTHREADS - number_of_partitions + 1].cleanEnv, thread[MAXTHREADS - number_of_partitions + 1].env, sizeof(thread[MAXTHREADS - number_of_partitions + 1].env));
            //reduce the problem
            stack_partition(number_of_partitions - 1);
        } else {
            (*thread[currentThread].f) (thread[currentThread].p);
            MyExitThread();
        }
    }
}

void MyInitThreads ()
{
    int i;

    if (MyInitThreadsCalled) {                /* run only once */
        Printf ("InitThreads: should be called only once\n");
        Exit ();
    }

    for (i = 0; i < MAXTHREADS; i++) {      /* initialize thread table */
        thread[i].valid = 0;
        queue.items[i] = -1;    /* initialize queue items array */
    }
    //save SP for thread 0
    if (setjmp(thread[0].env) == 0) {
        memcpy(thread[0].cleanEnv, thread[0].env, sizeof(thread[0].env));
        //reserve stack space
        stack_partition(MAXTHREADS);
        thread[0].valid = 1;                    /* initialize thread 0 */
        //initialize queue
        queue.front = 0;
        queue.end = 0;
        //initialize thread trackers
        currentThread = 0;
        lastSpawned = 0;
        spawnedThreads = 0;
        allSpawned = 0;
        exiting = 0;
        MyInitThreadsCalled = 1;
    } else {
        (*thread[0].f) (thread[0].p);
        MyExitThread();
    }
}


/*      MySpawnThread (func, param) spawns a new thread to execute
 *      func (param), where func is a function with no return value and
 *      param is an integer parameter.  The new thread does not begin
 *      executing until another thread yields to it.
 */

int MySpawnThread (func, param)
    void (*func)();         /* function to be executed */
    int param;              /* integer parameter */
{
    int i, j, available;

    if (! MyInitThreadsCalled) {
        Printf ("SpawnThread: Must call InitThreads first\n");
        Exit ();
    }

    if (spawnedThreads == MAXTHREADS - 1) {
        allSpawned = 1;
    }
    //go through array from last spawned to find available slot
    available = 0;
    j = lastSpawned;
    for (i = 0; i < MAXTHREADS; i++) {
        if (j == MAXTHREADS - 1) {
            j = 0;
        } else {
            j++;
        }
        if (!thread[j].valid) {
            available = 1;
            break;
        }
    }

    //if nothing was available, return -1
    if (!available) {
        return (-1);
    }

    //make lastSpawned store the available slot
    lastSpawned = j;

    //assign things for this thread
    thread[j].f = func;
    thread[j].p = param;
    thread[j].valid = 1;    /* mark the entry for the new thread valid */

    //if every space has already been used, refresh the enviornment for new thread
    if (allSpawned) {
        memcpy(thread[j].env, thread[j].cleanEnv, sizeof(thread[j].cleanEnv));
    }

    //put the thread id into a FIFO queue
    queue.items[queue.end] = j;
    if (queue.end == MAXTHREADS - 1) {
        queue.end = 0;
    } else {
        queue.end++;
    }

    spawnedThreads++;

    return (j);             /* done spawning, return new thread ID */
}

/*      MyYieldThread (t) causes the running thread to yield to thread t.
 *      Returns ID of thread that yielded to t (i.e., the thread that called
 *      MyYieldThread), or -1 if t is an invalid ID.
 */

int MyYieldThread (t)
    int t;                          /* thread being yielded to */
{
    int numToMove,
        changing,
        i,
        j;

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

    yielding = currentThread;
    if (yielding == t) {
        return (yielding);
    }

    //yielded to should go to the front of the queue
    for (i = 0; i < MAXTHREADS; i += 1) {
        if (queue.items[i] == t) {
            break;
        }
    }
    //remove it from the queue and move trailing threads a spot nearer the front
    queue.items[i] = -1;
    //find the number of items to shift in the array
    if (i <= queue.end) {
        numToMove = queue.end - i - 1;
    } else {
        numToMove = (MAXTHREADS - i) + (i - queue.end);
    }
    //move the items over
    changing = i;
    for (j = 0; j < numToMove; j++) {
        if (changing == MAXTHREADS - 1) {
            queue.items[changing] = queue.items[0];
            queue.items[0] = -1;
            changing = 0;
        } else {
            queue.items[changing] = queue.items[changing + 1];
            queue.items[changing + 1] = -1;
            changing++;
        }
    }
    //decrement the end
    if (queue.end == 0) {
        queue.end = MAXTHREADS - 1;
    } else {
        queue.end--;
    }

    //put currently running process to the end of the queue if not an exiting process
    if (!exiting) {
        queue.items[queue.end] = currentThread;
        if (queue.end == MAXTHREADS - 1) {
            queue.end = 0;
        } else {
            queue.end++;
        }
    }
    exiting = 0;

    //jump to the thread being yielded to
    if (setjmp (thread[currentThread].env) == 0) {
        currentThread = t;
        longjmp (thread[t].env, 1);
    }
    return (yielding);
}

/*      MyGetThread () returns ID of currently running thread.
*/

int MyGetThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("GetThread: Must call InitThreads first\n");
        Exit ();
    }

    return currentThread;
}

/*      MySchedThread () causes the running thread to simply give up the
 *      CPU and allow another thread to be scheduled.  Selecting which
 *      thread to run is determined here.  Note that the same thread may
 *      be chosen (as will be the case if there are no other threads).
 */

void MySchedThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("SchedThread: Must call InitThreads first\n");
        Exit ();
    }

    //if the only process running and exiting, exit program
    if (exiting && queue.items[queue.front] == -1) {
        Exit();
        //if the only running process and not exiting, yield to itself to finish
    } else if (queue.items[queue.front] == -1) {
        MyYieldThread(currentThread);
        //schedule the next item in the queue
    } else {
        MyYieldThread(queue.items[queue.front]);
    }
}

/*      MyExitThread () causes the currently running thread to exit.
*/

void MyExitThread ()
{
    int i = 0;

    if (! MyInitThreadsCalled) {
        Printf ("ExitThread: Must call InitThreads first\n");
        Exit ();
    }

    thread[currentThread].valid = 0;
    exiting = 1;

    //see which thread to schedule next
    MySchedThread();
}
