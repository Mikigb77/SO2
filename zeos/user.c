#include <libc.h>

char buff[24];

int pid;

void foo() {}

int __attribute__((__section__(".text.main")))
main(void)
{
  char stack[1024 * 4];

  clone(&foo, stack);

  /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
  /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  while (1)
  {
  }
}
