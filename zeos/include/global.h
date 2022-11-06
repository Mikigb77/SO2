#ifndef _MY_GLOBAL_
#define _MY_GLOBAL_

/**
 * includs all my global variables declarations.
 */
#include <list.h>
#include <sched.h>

extern unsigned long zeos_ticks;
extern struct list_head freequeue;
extern struct list_head readyqueue;
extern struct task_struct *idle_task;
extern unsigned long newPID;
extern int remainingQuantum;

// the full quantum size
#define FULL_QUANTUM 10

/****************************/
/**********Debug*************/
void check_list(struct list_head *h);
/****************************/

#endif