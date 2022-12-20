#include <libc.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{
  printf("\nstarting ticks: ");
  itoa(gettime(), buff);
  printf(buff);
  printf("\n");
  int a = 0;
  for (int i = 0; i < 9999999; ++i)
    a += i;
  printf("ending ticks: ");
  itoa(gettime(), buff);
  printf(buff);
  printf("\n");
  /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
  /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  while (1)
  {
  }
}
