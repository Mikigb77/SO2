#ifndef P_STATS_H_
#define P_STATS_H_
#include <stats.h>

/**
 * @brief initializes all stats
 *
 * @param s pointer to the task struct to initialize
 */
void init_stats(struct stats *s);

/**
 * @brief updates the state ticks and the total ticks up to the state change that triggered the function
 *
 * @param v value to update (the state_ticks_counter)
 * @param elapsed the time up to the last state change
 */
void update_stats(unsigned long *v, unsigned long *elapsed);

#endif