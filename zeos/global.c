#include <list.h>
#include "global.h"
#include <sched.h>

/// @brief the number of ticks elapsed since the boot of the system
int zeos_ticks = 0;

struct list_head freequeue;
struct list_head readyqueue;

/// @brief task struct of idle process
struct task_struct *idle_task;

// aux:

int global_pid = 0;

int get_pid()
{
    return ++global_pid;
}