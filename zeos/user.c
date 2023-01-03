#include <libc.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{

  pid = fork();
  if (pid == 0)
  {
    printf("\nson\n");
    exit();
  }
  else
  {
    printf("\nparent\n");
  }
  clean_screen();
  struct stats st;
  struct stats *stp = &st;
  get_stats(getpid(), stp);
  print_stats(stp);
  /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
  /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  while (1)
  {
  }
}
