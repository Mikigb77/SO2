#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define true 1
#define false 0

#define FULL_QUANTUM 10

extern int quantum;

extern int zeos_ticks;
extern struct list_head freequeue;
extern struct list_head readyqueue;

extern struct task_struct *idle_task;

/**
 * @brief Get the next pid (in the first call it returns 1)
 *
 * @return int
 */
int get_new_pid();

#endif