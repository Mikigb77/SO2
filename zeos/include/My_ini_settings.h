#ifndef _MY_INI_SET_
#define _MY_INI_SET_

/**
 * @param cs CodeSegment(Kernel)
 * @param esp @KernelStack
 * @param handler @handler
 */
extern void setMSR(int cs, int esp, void (*handler)());
/**
 * @brief changes the reg of the kernel_stack dir (0x175) to the esp0
 * @param esp0 The @ value of the kernel_stack;
 */
extern void setMSR_ESP0(int esp0);

#endif