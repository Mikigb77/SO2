#include "MyScheduler.h"

void update_sched_data_rr(void)
{
    ++ticks;
}
int needs_sched_rr(void)
{
    if (ticks >= 100)
    {
        ticks = 0;
        return 1;
    }
    return 0;
}
void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue)
{
    if (dst_queue == NULL)
        return;
    list_head *l = &t->list;
}