#include <asm.h>
#include <segment.h>

ENTRY(Suma)
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax
    inc %eax
    popl %ebp
    ret
