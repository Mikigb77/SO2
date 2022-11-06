
#ifndef _MY_SCHEDULER_
#define _MY_SCHEDULER_

#include "global.h"
#include <sched.h>

/**
 * @brief Update the relevant info for the scheduling desicions
 */
void update_sched_data_rr(void);

/**
 * @brief Decides if the task needs to be changed
 *
 * @return 1 (true) if the task has to be changed. 0 if not.
 */
int needs_sched_rr(void);

/**
 * @brief if the task is not running it updates the state of the task (ex: from running to ready).
 * @param t the task to update
 * @param dst_queue the list where the task will be moved
 */
void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue);

/**
 * @brief Decides the next proces to be executed and invokes the context switch
 */
void sched_next_rr(void);

/**
 * @brief Manages all the scheduler tasks like updateing the info or changing the current task for a new one
 */
void schedule(void);

#endif