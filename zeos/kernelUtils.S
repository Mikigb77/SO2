#include <asm.h>
#include <segment.h>

ENTRY(setMSR)
    pushl %ebp
    movl %esp, %ebp
    xor %edx, %edx
    movl 8(%ebp), %ecx
    movl 12(%ebp), %eax
    wrmsr
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY(task_switch)
    pushl %ebp
    movl %esp, %ebp
    pushl %esi
    pushl %edi
    pushl %ebx
    pushl 8(%ebp)
    call inner_task_switch
    popl %ebx
    popl %ebx
    popl %edi
    popl %esi
    popl %ebp   
    ret

/* cheat to get the %EBP */
ENTRY(get_ebp)
    movl %ebp, %eax
    ret

/* void switch_stack(int* old_kernel_ebp, int new_kernel_ebp) */
ENTRY(switch_stack)
    movl 4(%esp), %eax
	movl %ebp, (%eax)
	movl 8(%esp), %esp
	popl %ebp
	ret