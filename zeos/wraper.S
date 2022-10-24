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

ENTRY(write)
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

ENTRY(fast_write)
	pushl %ebp
	movl %esp, %ebp

	// Save edx and ecx
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %esi

	movl 8(%ebp), %ebx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %edx

	movl $4, %eax

	// Save return address
	leal wrEnd, %esi
	pushl %esi
	
	// Prepare stack frame for the syscall
	pushl %ebp
	movl %esp, %ebp

	sysenter
wrEnd:
	popl %ebp

	popl %esi

	popl %esi
	popl %ebx
	popl %ecx
	popl %edx
	jmp err_handling

ENTRY(gettime)
  pushl %ebp
  movl %esp, %ebp
  movl $10, %eax
  int $0x80
  popl %ebp
  ret
