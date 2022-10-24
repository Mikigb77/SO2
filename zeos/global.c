#include "global.h"
unsigned long zeos_ticks;

// the freequeue for the system
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;
