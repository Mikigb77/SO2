/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#include "global.h"

#include "My_ini_settings.h"

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd != 1)
    return -9; /*EBADF*/
  if (permissions != ESCRIPTURA)
    return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
  return -38; /*ENOSYS*/
}

int sys_getpid()
{
  return current()->PID;
}

int sys_fork()
{
  int PID = -1;
  /**check if there are freetasks to create the process*/
  if (list_empty(freequeue))
    return -MYENFRETSK;

  /*first get the parent tu (curr)*/
  struct task_struct *t = current();
  union task_union *tu = (union task_union *)t;

  /*now we get the son*/
  struct list_head *l = list_first(freequeue);
  struct task_struct *t1 = list_head_to_task_struct(l);
  union task_union *tu1 = (union task_union *)t1;
  list_del(l);
  /*first get a PID for the son*/
  PID = getNewPID();

  /*copy the task so it's the same as the father*/
  *tu1 = *tu;
  t1->PID = PID;
  /*we can set the kernel_ebp, since the 'tu' allready have the stack[] filled with a return @ and the ebp*/
  t1->kernel_ebp = &tu1->stack[KERNEL_STACK_SIZE - 2];

  /*now we need to allocate the frames to store the user data*/
  int frames[NUM_PAG_DATA];

  for (int i = 0; i < NUM_PAG_DATA; ++i)
  {
    frames[i] = alloc_frame();
    if (frames[i] == -1)
    {
      for (i; i >= 0; --i)
        free_frame(frames[i]);
      return -ENOMEM;
    }
  }
  /*now we have the frames alocated and only need to copy the data*/
  /*first let's allocate the new page table*/
  allocate_DIR(t1);
  /*get the page table of the curr and child*/
  page_table_entry *p1 = get_PT(t1); // page table for the child
  page_table_entry *p = get_PT(t);   // page table for the parent

  /*now we need to copy the content of the pages*/
  /*For the code pages we only need to copy the entry of the parent since they are read only */
  for (int i = 0; i < NUM_PAG_CODE; ++i)
    p1[PAG_LOG_INIT_CODE + i].entry = p[PAG_LOG_INIT_CODE + i].entry;

  /*with this they should be sheared*/
  /*now we need to copy the pages from the user data*/

  return PID;
}

void sys_exit()
{
}
int sys_write(int fd, char *buff, int size)
{
  // Check the user permissions:
  int error = 0;

  if ((error = check_fd(fd, ESCRIPTURA)) < 0)
    return error;

  if (buff == NULL)
    return -EFAULT;

  if (size < 0)
    return -EINVAL;

  /**
   * now we need to verify if the buff points to a readable direction
   * before attempting to copy it in the SO memoryspace
   */
  if (!access_ok(VERIFY_READ, buff, size))
    return -EACCES;

  char buffer[size];
  copy_from_user(buff, buffer, size);
  return sys_write_console(buffer, size);
}

unsigned long sys_gettime()
{
  return zeos_ticks;
}
