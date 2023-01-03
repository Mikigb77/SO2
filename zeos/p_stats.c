#include "p_stats.h"
#include "utils.h"

void init_stats(struct stats *s)
{
    s->user_ticks = 0;
    s->system_ticks = 0;
    s->blocked_ticks = 0;
    s->ready_ticks = 0;
    s->elapsed_total_ticks = get_ticks();
    s->total_trans = 0;
    s->remaining_ticks = get_ticks();
}

void update_stats(unsigned long *v, unsigned long *elapsed)
{
    unsigned long curr_ticks = get_ticks();
    *v += curr_ticks - *elapsed;
    *elapsed = curr_ticks;
}