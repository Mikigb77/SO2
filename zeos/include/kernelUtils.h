#ifndef _KERNEL_UTILS_
#define _KERNEL_UTILS_

/**
 * @brief set the MSR reg with the passed value
 *
 * @param reg register to set ex: 0x176 (handler for fast syscalls)
 * @param value the value to set the register with
 */
void setMSR(int reg, int value);

#endif