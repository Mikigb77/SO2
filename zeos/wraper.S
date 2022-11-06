/*
* My Wrapers
*/
#include <asm.h>
#include <segment.h>
/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %ebx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %edx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/


 err_handling:
  cmpl $0, %eax
  jge enderr
  neg %eax
  movl %eax, errno
  movl $-1, %eax
  enderr:
  movl %ebp, %esp
  popl %ebp
  ret

ENTRY(int_write)
  pushl %ebp
  movl %esp, %ebp
  /** Posem els parametres en els reg */
  movl 8(%ebp), %ebx
  movl 12(%ebp), %ecx
  movl 16(%ebp), %edx
  movl $4, %eax
  int $0x80
  /** now we need to check if there was an error */
 jmp err_handling

ENTRY(write)
	pushl %ebp
	movl %esp, %ebp

	/**save edx and ecx*/
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %esi
	/**add param to reg */
	movl 8(%ebp), %ebx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %edx
	movl $4, %eax

	/**save return address*/
	leal wrEnd, %esi
	pushl %esi
	
	/**prepare stack frame for the syscall*/
	pushl %ebp
	movl %esp, %ebp

	sysenter

wrEnd:
	popl %ebp
	/**extra pop for the leal */
	popl %esi
	popl %esi
	popl %ebx
	popl %ecx
	popl %edx
	jmp err_handling


ENTRY(getpid)
	pushl %ebp
	movl %esp, %ebp
	
	/**save the ebx and ecx that are used in the sysexit */
	pushl %ebx
	pushl %ecx
	/**save the return address */
	leal getpidEnd, %eax
	pushl %eax

	/**get the fake link for the system */
	pushl %ebp
	movl %esp, %ebp

	/**add the num of syscall to %eax */
	movl $20, %eax

	sysenter

getpidEnd:
	popl %ebp
	/**extra pop because of the push for the getpidEnd */
	popl %ecx
	popl %ecx
	popl %ebx

	/**finaly we go to the error handling section */
	jmp err_handling

ENTRY(fork)
	pushl %ebp
	movl %esp, %ebp

	/**since we use sysenter we need to save the ebx and ecx */
	pushl %ebx
	pushl %ecx
	
	/**now we need to save the return @ in the stack */
	leal forkEnd, %eax
	pushl %eax

	/**finaly we need to complete the activation bloc with the fake link */
	pushl %ebp
	movl %esp, %ebp

	/**finaly we add to the %eax the syscall number (2) */
	movl $2, %eax

	sysenter

forkEnd:
	/**recover the data */
	popl %ebp
	/**extrapop because of the leal */
	popl %ecx
	popl %ecx
	popl %ebx

	/**finaly we just need to go handle the errors */
	jmp err_handling

ENTRY(exit)
	pushl %ebp
	movl %esp, %ebp

	/**since we use sysenter we need to save the ebx and ecx */
	pushl %ebx
	pushl %ecx

	/**now we need to save the return @ in the stack */
	leal exitEnd, %eax
	pushl %eax

	/**finaly we need to complete the activation bloc with the fake link */
	pushl %ebp
	movl %esp, %ebp
	/**finaly we add to the %eax the syscall number (1) */
	movl $1, %eax

	sysenter

exitEnd:
	/**recover the data */
	popl %ebp
	/**extrapop because of the leal */
	popl %ecx
	popl %ecx
	popl %ebx

	/**finaly we just need to go handle the errors */
	jmp err_handling


ENTRY(gettime)
  pushl %ebp
  movl %esp, %ebp
  movl $10, %eax
  int $0x80
  popl %ebp
  ret
