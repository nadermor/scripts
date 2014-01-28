#include <setjmp.h>
#include <string.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h"

//16-bit addressing - 65536 bytes
#define STACKSIZE 65536     /* maximum size of thread stack */
#define THREAD_ZERO 0

#define TRUE 1
#define FALSE 0

static int MyInitThreadsCalled = 0; /* 1 if MyInitThreads called, else 0 */

//Global handle on the current running thread
static int currentRunningThread;

//Index for last assigned thread for MySpawnThread
//Starts at lastAssignedThread + 1
static int lastAssignedThreadID;

static int previousThread;

//Global count of currently active threads
static int activeThreadCount;

static struct thread {          /* thread table */
    int valid;
    
    jmp_buf currentEnv; /* current context */
    jmp_buf initialEnv; /* initial context */
    
    void (*threadFunc)();
    int funcParam;
} thread[MAXTHREADS];

//Queue for storing the order of thread execution
int threadQueue[MAXTHREADS-1];

void MyInitStack(int t)
{
    //Thread table full, go back to thread zero
    if (t == MAXTHREADS)
    {
        longjmp(thread[THREAD_ZERO].currentEnv, 1);
    }
    
    //Save the initial environment of the thread we just made
    if (setjmp(thread[t].initialEnv) == 0)
    {
        char s[STACKSIZE];
        
        //Reference s so compiler can detect it
        if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
            Printf ("Stack space reservation failed\n");
            Exit ();
        }
        
        MyInitStack(t+1);
    }
    
    (*(thread[currentRunningThread].threadFunc)) (thread[currentRunningThread].funcParam);
    
    MyExitThread();
}

void queueDump()
{
    Printf("| %d ", threadQueue[0]);
    Printf("| %d ", threadQueue[1]);
    Printf("| %d ", threadQueue[2]);
    Printf("| %d ", threadQueue[3]);
    Printf("| %d ", threadQueue[4]);
    Printf("| %d ", threadQueue[5]);
    Printf("| %d ", threadQueue[6]);
    Printf("| %d ", threadQueue[7]);
    Printf("| %d |\n", threadQueue[8]);
}

/*  MyInitThreads () initializes the thread package.  Must be the first
 *  function called by any user program that uses the thread package.
 */
void MyInitThreads ()
{
    int i;

    thread[THREAD_ZERO].valid = TRUE;

    for (i = 1; i < MAXTHREADS; i++) {  /* all other threads invalid */
        thread[i].valid = FALSE;
    }
    
    int k;
    //Initialize the thread queue
    for (k = 0; k < MAXTHREADS - 1; k++)
    {
        threadQueue[k] = -1;
    }
    
    currentRunningThread = THREAD_ZERO;
    MyInitThreadsCalled = 1;
    lastAssignedThreadID = THREAD_ZERO;
    activeThreadCount = 1;
    
    previousThread = -1;
    
    if (setjmp(thread[THREAD_ZERO].currentEnv) == 0)
    {
        //Initialize the initialEnv for thread zero
        MyInitStack(THREAD_ZERO);
    }
}

/*  MySpawnThread (func, param) spawns a new thread to execute
 *  func (param), where func is a function with no return value and
 *  param is an integer parameter.  The new thread does not begin
 *  executing until another thread yields to it.
 */

int MySpawnThread (func, param)
    void (*func)();     /* function to be executed */
    int param;      /* integer parameter */
{
    if (! MyInitThreadsCalled) {
        Printf ("Must call MyInitThreads first\n");
        Exit ();
    }

    //If there are already max number of active threads, return -1
    if (activeThreadCount >= MAXTHREADS)
    {
        return -1;
    }
    
    int validThreadTableIndex;
    
    //Look for a free entry in the Thread table, starting from last assigned thread
    int threadTableIndex;
    
    if (lastAssignedThreadID >= (MAXTHREADS - 1))
    {
        lastAssignedThreadID = -1;
    }
    
    //Put newly assigned thread in the thread table
    for (threadTableIndex = lastAssignedThreadID + 1; threadTableIndex < MAXTHREADS; ((threadTableIndex + 1 + MAXTHREADS) % MAXTHREADS))
    {
        if (thread[threadTableIndex].valid == FALSE)
        {
            //Printf("Spawning thread %d\n", threadTableIndex);
            validThreadTableIndex = threadTableIndex;
            lastAssignedThreadID = threadTableIndex;
            thread[validThreadTableIndex].valid = TRUE;
            activeThreadCount++;
            //Printf("Spawned new thread %d\n", validThreadTableIndex);
            break;
        }

    }
    
    //Put newly assigned thread in the thread queue
    int threadQueueIndex;
    for (threadQueueIndex = 0; threadQueueIndex < MAXTHREADS - 1; threadQueueIndex++)
    {
        if (threadQueue[threadQueueIndex] == -1)
        {
            threadQueue[threadQueueIndex] = validThreadTableIndex;
            break;
        }
    }
    
    //Make sure any existence of the previous environment is overwritten
    memcpy(thread[validThreadTableIndex].currentEnv, thread[validThreadTableIndex].initialEnv, sizeof(jmp_buf));


    thread[validThreadTableIndex].threadFunc = func;
    thread[validThreadTableIndex].funcParam = param;

    //Return the newly assigned thread ID
    return validThreadTableIndex;
}

/*!
    @abstract Removes the first thread from the queue and shifts every thread in the queue
     to the left
 */
void removeFirstThreadAndShiftLeft()
{
    int i;
    for (i = 0; i < MAXTHREADS - 2; i++)
    {
        threadQueue[i] = threadQueue[i+1];
    }
    
    //Make last spot free
    threadQueue[MAXTHREADS-2] = -1;
}

/*  MyYieldThread (t) causes the running thread to yield to thread t.
 *  Returns id of thread that yielded to t (i.e., the thread that called
 *  MyYieldThread), or -1 if t is an invalid id.
 */
int MyYieldThread (t)
    int t;              /* thread being yielded to */
{
    if (!MyInitThreadsCalled) 
    {
        Printf ("Must call MyInitThreads first\n");
        Exit ();
    }

    if (!thread[t].valid) 
    {
        Printf ("YieldThread: Thread %d does not exist\n", t);
        return (-1);
    }
    
    previousThread = currentRunningThread;
        
    if (currentRunningThread == t)
    {
        return currentRunningThread;
    }
    
    //Find process t in queue
    int threadQueueIndex;
    for (threadQueueIndex = 0; threadQueueIndex < MAXTHREADS - 1; threadQueueIndex++)
    {
        if (threadQueue[threadQueueIndex] == t)
        {
            currentRunningThread = threadQueue[threadQueueIndex];
            break;
        }
    }
    
    //Shift everything down from where t existed
    int i;
    for (i = threadQueueIndex; i < MAXTHREADS - 2; i++)
    {
        threadQueue[i] = threadQueue[i+1];
    }
    //Make sure last spot is free
    threadQueue[MAXTHREADS-2] = -1;
    
    //Find free spot
    for (i = 0; i < MAXTHREADS - 1; i++)
    {
        if (threadQueue[i] == -1)
        {
            threadQueue[i] = previousThread;
        }
    }
            
    //If we are saving the context for the current running thread
    if (setjmp (thread[previousThread].currentEnv) == 0) 
    {   
        longjmp (thread[currentRunningThread].currentEnv, 1);
    }

    return previousThread;
}

/*  MyGetThread () returns id of currently running thread.
 */

int MyGetThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("Must call MyInitThreads first\n");
        Exit ();
    }
    
    return currentRunningThread;
}


void MySchedThread()
{
    if (! MyInitThreadsCalled) {
        Printf ("Must call MyInitThreads first\n");
        Exit ();
    }
    
    //Check if thread 0 is valid or not-
    if ((thread[THREAD_ZERO].valid == FALSE) && (activeThreadCount == 0))
    {
        //currentRunningThread = -1;
        Exit();
    }
    else if (activeThreadCount == 0)
    {
        //currentRunningThread = -1;
        Exit();
    }
    
    if (thread[threadQueue[0]].valid == FALSE)
    {
        return;
    }
    
    previousThread = currentRunningThread;
    currentRunningThread = threadQueue[0];
    
    removeFirstThreadAndShiftLeft();
    
    int i;
    //Enqueue it since it's not exiting
    if (thread[previousThread].valid == TRUE)
    {
        for (i = 0; i < MAXTHREADS - 1; i++)
        {
            if (threadQueue[i] == -1)
            {
                //Printf("Putting calling thread %d at queue index %d\n", callingThread, i);
                threadQueue[i] = previousThread;
                break;
            }
        }
    }
    
    if (setjmp(thread[previousThread].currentEnv) == 0)
    {
        longjmp(thread[currentRunningThread].currentEnv, 1);
    }
    
}

/*  MyExitThread () causes the currently running thread to exit.
 */
void MyExitThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("Must call MyInitThreads first\n");
        Exit ();
    }
    
    int exitingThread;
    
    exitingThread = currentRunningThread;
    //Printf("Exiting thread is %d\n", exitingThread);
        
    //Deallocate the thread's resources in the table
    thread[exitingThread].valid = FALSE;
    
    activeThreadCount--;
    
    MySchedThread();
}
