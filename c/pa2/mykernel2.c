/* mykernel2.c: your portion of the kernel (last modified 10/18/09)
 *
 *	Below are procedures that are called by other parts of the kernel.
 *	Your ability to modify the kernel is via these procedures.  You may
 *	modify the bodies of these procedures any way you wish (however,
 *	you cannot change the interfaces).
 * 
 */

#include "aux.h"
#include "sys.h"
#include "mykernel2.h"

#define TIMERINTERVAL 1	/* in ticks (tick = 10 msec) */

/*	A sample process table.  You may change this any way you wish.
 */
static struct {
	int valid;		/* is this entry valid: 1 = yes, 0 = no */
	int pid;		/* process id (as provided by kernel) */
	float requested; // "m/n, given in MyReqCPU" -> youtube video
	float utilization; //"running time / alive time" -> youtube video
	float ratio; // "utilization/requested" -> project2_part2.pdf
	int runningTime; //"Increment when we schedule that process" -> youtube video
	int aliveTime; //"Increment EVERY QUANTUM that passes for all existing valid process" -> youtube video
	int quantumStart; //keep track of when a process started

} proctab[MAXPROCS];
static int proctab_index;

static struct {
	int valid;
	int pid;
} fifo[MAXPROCS];
static int fifo_index;

static struct {
	int valid;
	int pid;
} lifo[MAXPROCS];
static int lifo_index;

static struct {
	int valid;
	int pid;
} roundrobin[MAXPROCS];
static int roundrobin_index;


static int global_index;
static int global_timer;
static float cpu_alloc;


/*	InitSched () is called when kernel starts up.  First, set the
 *	scheduling policy (see sys.h).  Make sure you follow the rules
 *	below on where and how to set it.  Next, initialize all your data
 *	structures (such as the process table).  Finally, set the timer
 *	to interrupt after a specified number of ticks.
 */

void InitSched ()
{
//	Printf("Calling InitSched()\n");

	int i;

	

	/* First, set the scheduling policy.  You should only set it
	 * from within this conditional statement.  While you are working
	 * on this assignment, GetSchedPolicy will return NOSCHEDPOLICY,
	 * and so the condition will be true and you may set the scheduling
	 * policy to whatever you choose (i.e., you may replace ARBITRARY).
	 * After the assignment is over, during the testing phase, we will
	 * have GetSchedPolicy return the policy we wish to test, and so
	 * the condition will be false and SetSchedPolicy will not be
	 * called, thus leaving the policy to whatever we chose to test.
	 */
	if (GetSchedPolicy () == NOSCHEDPOLICY) {	/* leave as is */
		SetSchedPolicy (FIFO);		/* set policy here */
	}
		
	/* Initialize all your data structures here */
	for (i = 0; i < MAXPROCS; i++) {
		//Printf("Setting proctab[%d].valid to 0\n", i);
		proctab[i].valid = 0;
		roundrobin[i].valid = 0;
		lifo[i].valid = 0;
		fifo[i].valid = 0;
		proctab[i].valid = 0;
		proctab[i].requested = 1;
		proctab[i].runningTime = 0;
		proctab[i].aliveTime = 0; 
		proctab[i].utilization = 0;
		proctab[i].ratio = 1;
	}


	/* Set the timer last */
//	Printf("Setting TIMERINTERVAL to %d\n", TIMERINTERVAL);
	SetTimer (TIMERINTERVAL);
}


/*	StartingProc (pid) is called by the kernel when the process
 *	identified by pid is starting.  This allows you to record the
 *	arrival of a new process in the process table, and allocate
 *	any resources (if necessary).  Returns 1 if successful, 0 otherwise.
 */


int StartingProc (pid)
	int pid;
{
//	Printf("Fork()!\n");
//	Printf("Calling StartingProc(%d)\n", pid);

	
	/*
	 * These if-statements will execute depending on the scheduling policy
	 */
	if (!roundrobin[roundrobin_index].valid)
	{
		//Printf("Setting roundrobin[%d].valid = 1\n", p);
		roundrobin[roundrobin_index].valid = 1;
		//Printf("Setting roundrobin[%d].pid = %d\n", roundrobin_index, pid);
		roundrobin[roundrobin_index].pid = pid;
		
		roundrobin_index = (roundrobin_index + 1) % MAXPROCS;
		//Printf("Setting the next roundrobin_index to %d\n", roundrobin_index);

	}
	
	if (!lifo[lifo_index].valid)
	{
		//Printf("Setting lifo[%d].valid = 1\n", lifo_index);
		lifo[lifo_index].valid = 1;
		//Printf("Setting lifo[%d].pid = %d\n", lifo_index, pid);
		lifo[lifo_index].pid = pid;
		///Printf("Incrementing lifo_index to %d\n", lifo_index+1);
		lifo_index++;
		
		//"any newly created process should run immediately."
		//this means call DoSched()
		//Printf("Calling DoSched()\n");
		DoSched();

	}

	if (!fifo[fifo_index].valid)
	{
		//Printf("Setting fifo[%d].valid = 1\n", fifo_index);
		fifo[fifo_index].valid = 1;
		//Printf("Setting fifo[%d].pid = %d\n", fifo_index, pid);
		fifo[fifo_index].pid = pid;
		//Printf("Incrementing fifo_index to %d\n", fifo_index+1);
		fifo_index++;
	}

	int i = 0;
	for (i = 0; i < MAXPROCS; i++) {
		if (!proctab[i].valid) {
			//Printf("Setting proctab[%d].valid to 1\n", i);
			proctab[i].valid = 1;
			//Printf("Setting proctab[%d].pid to %d\n", i, pid);
			proctab[i].pid = pid;
//			Printf("Putting %d inside proctab[%d].quantumStart\n", global_timer, i);
			proctab[i].quantumStart = global_timer;
			return (1);
		}
	}

	//Printf ("Error in StartingProc: no free table entries\n");
	return (0);
}
			

/*	EndingProc (pid) is called by the kernel when the process
 *	identified by pid is ending.  This allows you to update the
 *	process table accordingly, and deallocate any resources (if
 *	necessary).  Returns 1 if successful, 0 otherwise.
 */


int EndingProc (pid)
	int pid;
{
//	Printf("Calling EndingProc(%d)\n",pid);
	int i;

	for (i = 0; i < MAXPROCS; i++)
	{
		if(roundrobin[i].valid && roundrobin[i].pid == pid)
		{
			//Printf("Setting roundrobin[%d].valid to 0\n",i);
			roundrobin[i].valid = 0;
			//Printf("Decrementing static var roundrobin_index\n");
			roundrobin_index--;
		}
	}

	for (i = 0; i < MAXPROCS; i++)
	{
		if(lifo[i].valid && lifo[i].pid == pid)
		{
			//Printf("Setting lifo[%d].valid to 0\n",i);
			lifo[i].valid = 0;
			//Printf("Decrementing static var lifo_index\n");
			lifo_index--;
		}
	}

	for (i = 0; i < MAXPROCS; i++) {
		if (fifo[i].valid && fifo[i].pid == pid) {
			//Printf("Setting fifo[%d].valid to 0\n",i);
			fifo[i].valid = 0;
		}
	}


	for (i = 0; i < MAXPROCS; i++) {
		if (proctab[i].valid && proctab[i].pid == pid) {

		//	Printf("Ending Process %d\n", pid);
	//		Printf("Process %d ended with %f utilization\n", pid, proctab[i].utilization);
			//Printf("Setting proctab[%d].valid to 0\n",i);		
			proctab[i].valid = 0;
			//cpu_alloc = cpu_alloc - proctab[i].utilization;
			return(1);
		}
	
		
	}

	//Printf ("Error in EndingProc: can't find process %d\n", pid);
	return (0);
}


/*	SchedProc () is called by kernel when it needs a decision for
 *	which process to run next.  It calls the kernel function
 *	GetSchedPolicy () which will return the current scheduling policy
 *	which was previously set via SetSchedPolicy (policy). SchedProc ()
 *	should return a process id, or 0 if there are no processes to run.
 */

int SchedProc ()
{
	global_timer++;
	//Printf("Calling SchedProc()\n");
	int i, j, k = 0;
	int value = proctab[global_index].ratio;
	
	switch (GetSchedPolicy ()) {

	case ARBITRARY:

		for (i = 0; i < MAXPROCS; i++) {
			if (proctab[i].valid) {
				//Printf("The process to run next is %d\n", proctab[i].pid);
				return (proctab[i].pid);
			}
		}
		break;

	case FIFO:

		/* your code here */
		for (i = 0; i < MAXPROCS; i++) {
			if (fifo[i].valid) {
				//Printf("The process to run next is %d\n", fifo[i].pid);
				return (fifo[i].pid);
			}
		}
		break;

	case LIFO:

		/* your code here */
		//Printf("lifo[%d].valid is %d\n", lifo_index-1, lifo[lifo_index-1].valid);	
		if(lifo[lifo_index-1].valid)
		{
			//Printf("The process to run next is %d\n", lifo[lifo_index-1].pid);
			return(lifo[lifo_index-1].pid);
		}
	        //Printf("Breaking out of LIFO case in SchedProc()\n");	
		break;

	case ROUNDROBIN:
		/* your code here */
	
		for(i = global_index, j = 0; j < MAXPROCS; i = (i+1) % MAXPROCS, j++)
		{
			//Printf("Current value of roundrobin[%d].valid is %d\n", i, roundrobin[i].valid);
			if(roundrobin[i].valid)
			{
				//Printf("The process to run next is %d\n", roundrobin[i].pid);
				global_index = (i+1) % MAXPROCS;
				return (roundrobin[i].pid);
			}
		}
		//Printf("Breaking out of ROUNDROBIN case in SchedProc()\n");
		break;

	case PROPORTIONAL:

		/* your code here */
		for ( i = global_index, j = 0; j < MAXPROCS; i = (i+1)%MAXPROCS, j++)
		{
			//Printf("Current value of proctab[%d].valid is %d\n", i, proctab[i].valid);
			if(proctab[i].valid)
			{
			
			//Utilization = CPU time already got/age of the process -> project2_part2.pdf
			proctab[i].utilization = 
				((float)proctab[i].runningTime) / ((float)global_timer);
			
			//Ratio = Utilization/Requested -> project2_part2.pdf
			proctab[i].ratio = (float)proctab[i].utilization / (float)proctab[i].requested;
			}
			
			if(proctab[i].ratio < value)
			{
				value = proctab[i].ratio;
				global_index = i;	
			}	
		}

		//find the process with the lowest current ratio
		for( j = 0; j < MAXPROCS; j++)
		{
			if(proctab[global_index].valid)
			{
				proctab[global_index].runningTime++;
				return (proctab[global_index].pid);
			}
			else
			{
				global_index = (global_index + 1) % MAXPROCS;
			}
		}
		break;

	}
	//Printf("ABNORMAL RETURN: Returning 0 inside SchedProc()\n");	
	return (0);
}


/*	HandleTimerIntr () is called by the kernel whenever a timer
 *	interrupt occurs.
 */

void HandleTimerIntr ()
{
//	Printf("Calling HandleTimerIntr()\n");
	SetTimer (TIMERINTERVAL);
	DoSched ();		/* make scheduling decision */
}

/*	MyRequestCPUrate (pid, m, n) is called by the kernel whenever a process
 *	identified by pid calls RequestCPUrate (m, n).  This is a request for
 *	a fraction m/n of CPU time, effectively running on a CPU that is m/n
 *	of the rate of the actual CPU speed.  m of every n quantums should
 *	be allocated to the calling process.  Both m and n must be greater
 *	than zero, and m must be less than or equal to n.  MyRequestCPUrate
 *	should return 0 if successful, i.e., if such a request can be
 *	satisfied, otherwise it should return -1, i.e., error (including if
 *	m < 1, or n < 1, or m > n).  If MyRequestCPUrate fails, it should
 *	have no effect on scheduling of this or any other process, i.e., as
 *	if it were never called.
 */

int MyRequestCPUrate (pid, m, n)
	int pid;
	int m;
	int n;
{
	int i;
	int value;

	//Printf("Calling MyRequestCPUrate(%d,%d,%d)\n", pid, m, n);
	/* your code here */
	//Printf("Current pid is %d\n", pid);
	//Printf("m/n is %f and cpu_alloc is %f\n", ((float)m/(float)n), cpu_alloc); 
	
	//Do initial checks
	//"MyRequestCPUrate (pid, m, n) should fail if either m or n 
	//are invalid (m < 1, or n < 1, or m > n)."
	if(m<1||n<1||m>n) return -1;
	
	/*
	* "It should also fail if either m or n if a process calls RequestCPUrate (m, n) 
	* such that it would result in over-allocating
 	* the CPU.  Over-allocation occurs if the sum of the rates requested by
 	* processes exceeds 100%.
	*/

	for(i = 0; i < MAXPROCS; i++)
	{
		if(proctab[i].valid)
		{
			cpu_alloc = cpu_alloc + proctab[i].requested;
		}
	}

	cpu_alloc = cpu_alloc + ( (float) m / (float) n);


	if (cpu_alloc > 1)
	{
		return -1;
	}

	for(i = 0; i < MAXPROCS; i++)
	{
		if(proctab[i].pid == pid && proctab[i].valid)
		{
			proctab[i].requested = ( (float) m ) / ( (float)n );
		}
	}

	if(GetSchedPolicy() == PROPORTIONAL) DoSched();
	return (0);
}
