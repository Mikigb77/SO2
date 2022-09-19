#include <asm.h>
ENTRY(addASM)
    PUSHL %ebp
    MOVL %esp, %ebp
    MOVL 8(%ebp), %eax
    addl 12(%ebp), %eax
    movl %ebp, %esp
    popl %ebp
    ret