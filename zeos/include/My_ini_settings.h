#ifndef _MY_INI_SET_
#define _MY_INI_SET_

/**
 * @param cs CodeSegment(Kernel)
 * @param esp @KernelStack
 * @param handler @handler
 */
extern void setMSR(int cs, int esp, void (*handler)());

#endif