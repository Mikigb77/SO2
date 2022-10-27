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
