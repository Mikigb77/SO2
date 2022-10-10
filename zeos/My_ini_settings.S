#include <asm.h>
ENTRY(setMSR)
    pushl %ebp
    movl %esp, %ebp
    /**make %edx = 0x00 */
    xor %edx, %edx
    /**set the values of reg to write into %ecx */
    /** it will write in the %ecx reg whatever it founds on %edx:%eax */

    /** code segment: */
    movl 8(%ebp), %eax
    movl $0x174, %ecx
    wrmsr

    /**Kernel ESP: */
    movl 12(%ebp), %eax
    movl $0x175, %ecx
    wrmsr

    /**Handler: */
    movl 16(%ebp), %eax
    movl $0x176, %ecx
    wrmsr
    
    movl %ebp, %esp
    popl %ebp
    ret
