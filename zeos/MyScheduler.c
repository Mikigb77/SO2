#include "MyScheduler.h"

/**************************/
/**********DEB*************/
#include <libc.h>
/**************************/

void update_sched_data_rr(void)
{
    --remainingQuantum;
}
int needs_sched_rr(void)
{
    if (remainingQuantum <= 0)
    {
        return 1;
    }
    return 0;
}

void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue)
{
    struct list_head *l = &t->list;
    if (t->state == ST_READY && dst_queue == NULL)
    {
        t->state = ST_RUN;
    }
    else
    {
        if (t->state == ST_BLOCKED)
            list_del(l);
        t->state = dst_queue == &readyqueue ? ST_READY : ST_BLOCKED;
        list_add_tail(l, dst_queue);
    }
}
/****************************/
/**********Debug*************/

void check_list(struct list_head *h)
{
    char buff[200];
    struct list_head *l = list_first(h);
    struct task_struct *t;
    printk("List elem:\n[");
    list_for_each(l, h)
    {
        t = list_head_to_task_struct(l);
        // if (t->PID == 0)
        //     list_del(l);
        itoa(t->PID, buff);
        printk(buff);
        printk(", ");
    }
    t = list_head_to_task_struct(l);
    // if (t->PID == 0)
    // list_del(l);
    itoa(t->PID, buff);
    printk(buff);
    printk("]\n");
}
/***************************/

void sched_next_rr(void)
{
    /************************/

    char buff[200];
    printk("\nCurrent PID: ");
    itoa(current()->PID, buff);
    printk(buff);
    printk("\n");
    check_list(&readyqueue);

    /************************/

    struct task_struct *t;
    // the new task
    if (!list_empty(&readyqueue))
    {
        struct list_head *l = list_first(&readyqueue);
        list_del(l);
        t = list_head_to_task_struct(l);
    }
    else
        t = idle_task;

    // initialize just in case:
    // t->state = ST_READY;

    /************************************/
    /************************************/

    printk("New PID: ");
    itoa(t->PID, buff);
    printk(buff);
    printk("\n");

    /************************************/

    // the current one:
    struct task_struct *t1 = current();
    // reassure that the current t->state is ST_RUN:
    // t1->state = ST_RUN;

    // we also update the remainingQuantum
    remainingQuantum = t->quantum;

    // need to be changed if blocked implemented!!!
    update_process_state_rr(t1, &readyqueue);

    // update the state of the proc:
    update_process_state_rr(t, NULL);

    // finally the task switch:
    task_switch((union task_union *)t);
}

void schedule(void)
{
    update_sched_data_rr();

    if (needs_sched_rr())
    {
        sched_next_rr();
    }
}
