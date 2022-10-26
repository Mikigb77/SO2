#include <asm.h>
ENTRY(task_switch_)
    pushl %ebp
    movl %esp, %ebp

    pushl %ebx
    pushl %edi
    pushl %esi

    pushl 8(%ebp)
    call inner_task_switch
    
    /**desapilem el 8(%ebp) */
    addl $4, %esp 

    popl %esi
    popl %edi
    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret

ENTRY(inner_task_switch)
    pushl %ebp
    movl %esp, %ebp

    /**Now we need to add the %ebp in the pcb['ebp'] */
    movl 8(%ebp), %eax /** eax = @ of union task_union */
    leal 4(%eax), %ebx /** @ kernel_ebp of new */
    
    call current /**eax = @ current */
    movl %ebp, 4(%eax) /** @ kernel_ebp (curr) = ebp */
    movl 4(%eax), %esp /** esp = @ kernel_ebp (new) */
    popl %ebp /**this->ebp(new) =  ebp*/
    ret