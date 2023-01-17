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



ENTRY(err_handling)
    test %eax, %eax
    jns end_err
    neg %eax
    movl %eax, errno
    movl $-1, %eax
end_err:
    ret

/* int write (int fd, char * buffer, int size); */
ENTRY(write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx
    movl $4, %eax
    call syscall_sysenter
    popl %ebx
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret

/* int gettime(); */
ENTRY(gettime)
    pushl %ebp
    movl %esp, %ebp
    movl $10, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret

/* int getpid() */
ENTRY(getpid)
    pushl %ebp
    movl %esp, %ebp
    movl $20, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret

/* int fork() */
ENTRY(fork)
    pushl %ebp
    movl %esp, %ebp
    movl $2, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret

/* void exit() */
ENTRY(exit)
    pushl %ebp
    movl %esp, %ebp
    movl $1, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret
    
/* int get_stats(int pid, struct stats *st) */
ENTRY(get_stats)
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl $35, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret

/* int clone(void (*fn)(void), char *stack); */
ENTRY(clone)
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl $120, %eax
    call syscall_sysenter
    call err_handling
    movl %ebp, %esp
    popl %ebp
    ret