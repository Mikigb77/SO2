#include <libc.h>
#include <stddef.h>

char buff[24];


int pid;

int printmf(char *buff){
  int n = write(1, buff,strlen(buff));
  if (n == -1){
    perror();
  }
  return n;
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
  char buff[200] = {' ','H','o','l','a', '!', '\n', '\n'};
  printmf(buff);
  printmf("ticks[0]: ");
  int a = gettime();
  itoa(a, buff);
  printmf(buff);
  printmf("\n");
  for (unsigned long i=0; i < 9999999; ++i);

  printmf("ticks[1]: ");
  int b = gettime();
  itoa(b, buff);
  printmf(buff);
  printmf("\n");
  printmf("Elapsed: ");
  itoa(b-a, buff);
  printmf(buff);
  printmf(" ticks");
  printmf("\n");

    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */


  while(1) { }
}
