#ifndef _MY_INI_SET_
#define _MY_INI_SET_

/**
 * @param cs CodeSegment(Kernel)
 * @param esp @KernelStack
 * @param handler @handler
 */
extern void setMSR(int cs, int esp, void (*handler)());
extern void setMSR_ESP0(int esp0);

#endif