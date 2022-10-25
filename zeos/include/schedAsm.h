#ifndef _SCHED_ASM_
#define _SCHED_ASM_

/**
 * @brief aux function for task switch
 * it also calls the inner_task_switch
 * @param t is the pointer to the new program
 */
extern void task_switch_(struct task_switch *t);

extern void inner_task_switch(struct task_switch *t);

#endif