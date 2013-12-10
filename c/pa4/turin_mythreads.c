#include <setjmp.h>

#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define QUEUESIZE 1000
#define DEBUG 0 
static int MyInitThreadsCalled = 0; /* 1 if MyInitThreads called, else 0 */
static int head = 1;
static int search_from = 1;
static int current_tid = 0;
static int spawning_tid = 0;
typedef struct{
    int q[QUEUESIZE -1];
    int first;
    int last;
    int pointer;
    int count;
}queue;

static queue tid_queue;

void init_queue(queue *q)
{
    q->first = 0;
    q->last = QUEUESIZE - 1;
    q->count = 0;
    q->pointer = 0;
}
void enqueue(queue *q, int x)
{
    if (q->count >= QUEUESIZE) Printf("queue overflow");
    else {
        q->last = (q->last+1) % QUEUESIZE;
        q->q[ q->last ] = x;
        q->count = q->count + 1;
    }
}
int dequeue(queue *q)
{
    int x;

    if (q->count <= 0) Printf("Warning: empty queue dequeue.\n");
    else {
        x = q->q[ q->first ];
        q->first = (q->first+1) % QUEUESIZE;
        q->count = q->count - 1;
    }

    return(x);
}

void print_queue(queue *q){
    int counter = 0;
    while( counter < q->count){
        counter++;
    }
}

void move_to_queue_head(queue *q, int t){
    int count = 0; 
    while(count < q->count){
        if(q->q[q->first + count] == t){
            if((q->first + count) == q->first){
                break;
            }
            int inner_count = count;
            while((q->first + inner_count) != q->last){
                q->q[q->first + inner_count] = q->q[q->first + inner_count + 1];
                inner_count += 1;
            }
            if(q->last > 1 && q->first > 0){
                q->last -= 1;
                q->q[q->first - 1] = t;
                q->first -= 1;
            }
        }
        count++;
    }
}

int get_queue_last(queue *q)
{
    if(q->count <= 0) Printf("Warning: empty queue dequeue.\n");
    return q->q[ q->last ];
}
int get_queue_first(queue *q)
{
    if(q->count <= 0) Printf("Warning: empty queue dequeue.\n");
    return q->q[ q->first ];
}
int get_queue_next(queue *q)
{
    if(q->count <= 0) Printf("Warning: empty queue dequeue.\n");
    int current = q->pointer;
    q->pointer = (q->pointer + 1) % q->count;
    return q->q[current];
}
int empty(queue *q)
{
    if (q->count <= 0) return 1;
    else return 0;
}

static struct thread {      /* thread table */
    int valid;      /* 1 if entry is valid, else 0 */
    jmp_buf env;      /* current context */
    jmp_buf clean_env;
    int clean;
    void *func;
    int param;
} thread[MAXTHREADS];
#define STACKSIZE 65536   /* maximum size of thread stack */

/*  MyInitThreads () initializes the thread package.  Must be the first
 *   *  function called by any user program that uses the thread package.
 *    */
void setStackSpace(int pos)
{
    if(pos < 1){
        thread[0].clean = 0;
        longjmp(thread[0].clean_env, 2);
    }
    char s[STACKSIZE];
    if( setjmp(thread[MAXTHREADS - pos].clean_env) == 0){
        if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
            Exit ();
        }
        setStackSpace(pos - 1);
    }else{

        thread[head].clean = 0;
        if (setjmp (thread[head].env) == 0) { /* save context of 1 */
            longjmp (thread[current_tid].env, 1); /* back to thread 0 */
        }

        void (*f)() = thread[current_tid].func; /* f saves func on top of stack */
        int p = thread[current_tid].param;    /* p saves param on top of stack */
        (*f) (p);     

        MyExitThread ();    /* thread 1 is done - exit */

    }

}

void MyInitThreads ()
{
    int i;
    int setjum_ret;

    thread[0].valid = 1;      /* the initial thread is 0 */
    thread[0].clean = 1;
    for (i = 1; i < MAXTHREADS; i++) {  /* all other threads invalid */
        thread[i].valid = 0;
        thread[i].clean = 1;
    }

    MyInitThreadsCalled = 1;
    init_queue(&tid_queue);
    enqueue(&tid_queue, 0);

    char s[STACKSIZE];
    setjum_ret = setjmp(thread[0].clean_env);
    if( setjum_ret == 0){
        if (((int) &s[STACKSIZE-1]) - ((int) &s[0]) + 1 != STACKSIZE) {
            Exit ();
        }
        setStackSpace(MAXTHREADS - 1);
    }else if(setjum_ret == 2){
        return;
    }else{
        thread[0].clean = 0;
        if (setjmp (thread[0].env) == 0) { /* save context of 1 */
            longjmp (thread[current_tid].env, 1); /* back to thread 0 */
        }

        void (*f)() = thread[current_tid].func; /* f saves func on top of stack */
        int p = thread[current_tid].param;    /* p saves param on top of stack */
        (*f) (p);     /* execute func (param) */
        MyExitThread();
        return;
    }

}

/*  MySpawnThread (func, param) spawns a new thread to execute
 *   *  func (param), where func is a function with no return value and
 *    *  param is an integer parameter.  The new thread does not begin
 *     *  executing until another thread yields to it.
 *      */

int MySpawnThread (func, param)
    void (*func)();   /* function to be executed */
    int param;    /* integer parameter */
{
    if (! MyInitThreadsCalled) {
        Printf ("MySpawnThread: Must call MyInitThreads first\n");
        Exit ();
    }

    for (int i = 0; i < MAXTHREADS; i++) {  /* all other threads invalid */
        if(thread[search_from].valid == 0){
            thread[search_from].valid = 1; /* mark the entry for the new thread valid */
            head = search_from;
            search_from = (search_from + 1) % MAXTHREADS;
            break;
        }
        search_from++;
        search_from = search_from % MAXTHREADS;
    }
    enqueue(&tid_queue, head);
    if (setjmp (thread[current_tid].env) == 0) {  /* save context of thread 0 */
        thread[head].func = func;
        thread[head].param = param;
        if(thread[head].clean == 1){
            longjmp(thread[head].clean_env, 1);
        }else{
            longjmp(thread[head].env, 1);
        }
    }

    return head;


}

/*  MyYieldThread (t) causes the running thread to yield to thread t.
 *   *  Returns id of thread that yielded to t (i.e., the thread that called
 *    *  MyYieldThread), or -1 if t is an invalid id.
 *     */

int MyYieldThread (t)
    int t;        /* thread being yielded to */
{
    int parent_thread = current_tid;
    if (! MyInitThreadsCalled) {
        Printf ("MyYieldThread: Must call MyInitThreads first\n");
        Exit ();
    }
    if( t < 0 || t > MAXTHREADS){
        return (-1);
    }
    if (! thread[t].valid) {
        Printf ("MyYieldThread: Thread %d does not exist\n", t);
        return (-1);
    }

    if( current_tid == t){
        return current_tid;
    }
    enqueue(&tid_queue, current_tid);
    dequeue(&tid_queue);
    move_to_queue_head(&tid_queue, t);
    print_queue(&tid_queue);
    if(DEBUG == 1) Printf("rearrange thread %d to first\n", t);

    if (setjmp (thread[current_tid].env) == 0) {
        current_tid = t;
        if(DEBUG == 1) Printf("yield to thread %d long jump %d\n", t, thread[t].env);
        longjmp (thread[t].env, 1);
    }else{
        return parent_thread;
    }

}



/*  MyGetThread () returns id of currently running thread.
 *   */

int MyGetThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("MyGetThread: Must call MyInitThreads first\n");
        Exit ();
    }
    return current_tid;
}

/*  MySchedThread () causes the running thread to simply give up the
 *   *  CPU and allow another thread to be scheduled.  Selecting which
 *    *  thread to run is determined here.  Note that the same thread may
 *     *  be chosen (as will be the case if there are no other threads).
 *      */

void MySchedThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("MySchedThread: Must call MyInitThreads first\n");
        Exit ();
    }
    int tid;
    if(tid_queue.count > 0){
        if(thread[current_tid].valid == 1){
            enqueue(&tid_queue, current_tid);
        }
        dequeue(&tid_queue);
        print_queue(&tid_queue);

        if(tid_queue.count == 0){
            Exit();
        }
        tid = get_queue_first(&tid_queue);
        if (setjmp (thread[current_tid].env) == 0) {
            current_tid = tid;
            longjmp(thread[tid].env, 1);
        }
    }else{
        Exit();
    }

}

/*  MyExitThread () causes the currently running thread to exit.
 *   */

void MyExitThread ()
{
    if (! MyInitThreadsCalled) {
        Printf ("MyExitThread: Must call MyInitThreads first\n");
        Exit ();
    }
    thread[current_tid].valid = 0;
    thread[current_tid].clean = 1;
    if(tid_queue.count > 0){
        print_queue(&tid_queue);
        MySchedThread();
    }
    Exit();
}
