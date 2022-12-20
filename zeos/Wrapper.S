#include <asm.h>
#include <segment.h>

ENTRY(syscall_sysenter)
    pushl %edx
    pushl %ecx
    pushl $SYSENTER_RETURN
    pushl %ebp
    movl %esp, %ebp
    sysenter

ENTRY(SYSENTER_RETURN)
    popl %ebp
    popl %ecx
    popl %ecx
    popl %edx
    ret



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

    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx

    /* passing the syscall number */
    movl $4, %eax

    call syscall_sysenter
    
    popl %ebx

    jmp err_handling