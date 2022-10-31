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
    list_head *l = &t->list;
    list_del(l);
    if (dst_queue == NULL)
        return;
    list_add_tail(dst_queue, l);
}

void sched_next_rr(void)
{
    /*first we get the new task union to implement*/
    list_head l = list_first(readyqueue);
    struct task_struct *t = list_head_to_task_struct(l);
    union task_union *tu = (union task_union *)t;

    /*now we need the current task union since we need to update it's state*/
    struct task_struct *tCurr = current();

    /*add the curr in a list (it could also be blocked but it's not implemented)*/
    update_process_state_rr(tCurr, &readyqueue);
    /*take out the new from the list it's currently in*/
    update_process_state_rr(t, NULL);

    /*finaly make the task switch*/
    task_switch(tu);
}