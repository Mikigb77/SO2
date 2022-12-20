#include <asm.h>
#include <segment.h>

err_handling:
    test %eax, %eax
    jns end_err
    neg %eax
    movl %eax, errno
    movl $-1, %eax
end_err:
    movl %ebp, %esp
    popl %ebp
    ret

/* int write (int fd, char * buffer, int size); */
ENTRY(write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %ecx
    pushl %edx

    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx

    /* passing the syscall number */
    movl $4, %eax

    int $0x80
    
    popl %edx
    popl %ecx
    popl %ebx

    jmp err_handling