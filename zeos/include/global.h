#ifndef _GLOBAL_H_
#define _GLOBAL_H_

extern int zeos_ticks;
extern struct list_head freequeue;
extern struct list_head readyqueue;

extern struct task_struct *idle_task;

/**
 * @brief Get the next pid (in the first call it returns 1)
 *
 * @return int
 */
int get_pid();

#endif