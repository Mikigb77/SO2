#include "entry.S"
#define EOI \
  movb $0x20, %al ; \
  outb %al, $0x20 ;
#define RESTORE_ALL\
  popl %ebx; \
  popl %ecx; \
  popl %edx; \
  popl %esi; \
  popl %edi; \
  popl %ebp; \
  popl %eax; \
  popl %ds; \
  popl %es; \
  popl %fs; \
  popl %gs; \

ENTRY (keyboard_handler)
  SAVE_ALL
  EOI
  call keyboard_routine
  RESTORE_ALL
  iret
