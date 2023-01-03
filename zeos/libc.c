/*
 * libc.c
 */

#include <libc.h>

#include <types.h>
#include "errno.h"
#include "errnomsg.h"

int errno;

void itoa(int a, char *b)
{
  int i, i1;
  char c;

  if (a == 0)
  {
    b[0] = '0';
    b[1] = 0;
    return;
  }

  i = 0;
  while (a > 0)
  {
    b[i] = (a % 10) + '0';
    a = a / 10;
    i++;
  }

  for (i1 = 0; i1 < i / 2; i1++)
  {
    c = b[i1];
    b[i1] = b[i - i1 - 1];
    b[i - i1 - 1] = c;
  }
  b[i] = 0;
}

int strlen(char *a)
{
  int i;

  i = 0;

  while (a[i] != 0)
    i++;

  return i;
}

void perror()
{
  char *buffer;

  buffer = errnomsg[errno - 1];

  write(1, buffer, strlen(buffer));
}

/*********************************************/
/*********************AUX*********************/

void printf(char *buff)
{
  if (write(1, buff, strlen(buff)) < 0)
    perror();
}

void print_stats(struct stats *st)
{
  printf("\nSTATS:\n");
  int vs[7];
  char *vb[7];
  vb[0] = "user_ticks";
  vs[0] = (int)st->user_ticks;
  vb[1] = "system_ticks";
  vs[1] = (int)st->system_ticks;
  vb[2] = "blocked_ticks";
  vs[2] = (int)st->blocked_ticks;
  vb[3] = "ready_ticks";
  vs[3] = (int)st->ready_ticks;
  vb[4] = "elapsed_total_ticks";
  vs[4] = (int)st->elapsed_total_ticks;
  vb[5] = "total_trans";
  vs[5] = (int)st->total_trans; /* Number of times the process has got the CPU: READY->RUN transitions */
  vb[6] = "remaining_ticks";
  vs[6] = (int)st->remaining_ticks;
  char buff[2000];
  for (int i = 0; i < 7; ++i)
  {
    printf(vb[i]);
    printf(": ");
    itoa(vs[i], buff);
    printf(buff);
    printf("\n");
  }
}

void clean_screen()
{
  int cols = 80;
  int rows = 25;

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
      printf(" ");
    printf("\n");
  }
}
/*********************************************/
