#ifndef P_STATS_H_
#define P_STATS_H_
#include <stats.h>
#include <sched.h>
enum stats_
{
    usr_to_sys,
    sys_to_usr,
    sys_to_red,
    red_to_sys
};

/**
 * @brief initialize all stats info
 *
 * @param t pointer to the task struct of the task that will get all stats initialized
 */
void init_stats(struct task_struct *t);

/**
 * @brief updates all the stats of the process
 *
 * @param dir the direction that it goes: usr_to_sys(0), sys_to_usr(1), sys_to_red(2) or red_to_sys(3)
 *
 */
void update_stats(int dir);

#endif