#include <asm.h>
#include <segment.h>

#Entry(Suma)
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax
    inc %eax
    popl %ebp
    ret
