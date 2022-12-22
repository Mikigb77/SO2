#ifndef _KERNEL_UTILS_
#define _KERNEL_UTILS_

/**
 * @brief set the MSR reg with the passed value
 *
 * @param reg register to set
 * ex:
 * 0x174 (@kernel CS)
 * 0x175 (@kernel stack)
 * 0x176 (handler for fast syscalls)
 * @param value the value to set the register with
 */
void setMSR(int reg, int value);

/**
 * @brief Get the ebp register
 *
 * @return int
 */
int get_ebp();

/**
 * @brief swiches the current stack with the value of the newone
 *
 * @param new_kernel_esp the new stack value of esp
 */
void switch_stack(int *old_kernel_ebp, int new_kernel_esp);

#endif