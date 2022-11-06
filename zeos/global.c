#include "global.h"
unsigned long zeos_ticks;

// the freequeue for the system
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;

// the newpid that will be assigned to the process
unsigned long newPID = 0;

// the quantum that will be updated when it gets to 0
int remainingQuantum = FULL_QUANTUM;
