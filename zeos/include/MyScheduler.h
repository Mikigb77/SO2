
#include <sched.h>

unsigned int ticks = 0;
void update_sched_data_rr(void);
int needs_sched_rr(void);
void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue);