#include "p_stats.h"

void init_stats(struct task_struct *t)
{
    struct stats *st = &t->stats;
}

void update_stats(int dir)
{
    switch (dir)
    {
    case usr_to_system:
        update_usr_to_sys();
        break;
    case sys_to_usr:
        update_sys_to_usr();
        break;
    case sys_to_red:
        update_sys_to_red();
        break;
    case red_to_sys:
        update_sys_to_red();
        break;
    default:
        break;
    }
}