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