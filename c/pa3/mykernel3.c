#include "aux.h"
#include "sys.h"
#include "mykernel3.h"

#define SIZE 1024

typedef struct{
int q[SIZE -1];
int head;
int tail;
int ptr;
int ctr;
}queue;

void newQueue(queue *q)
{
q->head = 0;
q->tail = SIZE - 1;
q->ctr = 0;
q->ptr = 0;
}

void push(queue *q, int x)
{
if (q->ctr >= SIZE)
{//empty
}
else {
q->tail = (q->tail+1) % SIZE;
q->q[ q->tail ] = x;
q->ctr = q->ctr + 1;
}
}

int pop(queue *q)
{
int x;

if (q->ctr <= 0) 
{//empty
}
else {
x = q->q[ q->head ];
q->head = (q->head+1) % SIZE;
q->ctr = q->ctr - 1;
}

return(x);
}


int getTail(queue *q)
{
return q->q[ q->tail ];
}
int getHead(queue *q)
{
return q->q[ q->head ];
}
int getNext(queue *q)
{
int curr = q->ptr;
q->ptr = (q->ptr + 1) % q->ctr;
return q->q[curr];
}
int empty(queue *q)
{
if (q->ctr <= 0) 
{
return 1;
}
else 
{
return 0;
}
}


static struct {
int valid;  
int value;        
queue fifo_queue;
} semtab[MAXSEMS];



void InitSem ()
{
int s;


for (s = 0; s < MAXSEMS; s++) {  
semtab[s].valid = 0;
}
}


int MySeminit (p, v)
int p, v;
{
int s;


for (s = 0; s < MAXSEMS; s++) {
if (semtab[s].valid == 0) {
break;
}
}
if (s == MAXSEMS) {
Printf ("No free semaphores\n");
return (-1);
}

semtab[s].valid = 1;
semtab[s].value = v;
newQueue(&(semtab[s].fifo_queue));
return (s);
}


void MyWait (p, s)
int p, s;
{
semtab[s].value--;
if (semtab[s].value < 0)
{
push(&(semtab[s].fifo_queue), p);
Block(p);
}
}


void MySignal (p, s)
int p, s;
{
semtab[s].value++;
if (semtab[s].value <= 0)
{
int unblock_pid = pop(&(semtab[s].fifo_queue));
Unblock(unblock_pid);
}
}
