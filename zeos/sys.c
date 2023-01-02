/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>
#include <sched.h>
#include <utils.h>
#include <io.h>
#include <mm.h>
#include <mm_address.h>
#include <list.h>
#include <sched.h>
#include "errno.h"
#include "global.h"
#include "kernelUtils.h"

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

int ret_from_fork()
{
  return 0;
}

int sys_fork()
{
  int PID = -1;
  if (list_empty(&freequeue))
    return -ENOMEM;
  struct list_head *l = list_first(&freequeue);
  list_del(l);
  struct task_struct *c = list_head_to_task_struct(l);
  union task_union *cu = (union task_union *)c;

  struct task_struct *p = current();
  union task_union *pu = (union task_union *)p;

  copy_data(pu, cu, sizeof(union task_union));

  // start dir:
  allocate_DIR(c);

  page_table_entry *pc = get_PT(c); // pt of son
  page_table_entry *pp = get_PT(p); // pt of parent

  // allocate the space:
  for (int i = 0; i < NUM_PAG_DATA; ++i)
  {
    int frame = alloc_frame();
    if (frame == -1)
    {
      for (int j = 0; j < i; ++j)
      {
        free_frame(get_frame(pc, PAG_LOG_INIT_DATA + j));
        del_ss_pag(pc, PAG_LOG_INIT_DATA + j);
      }
      return -ENOMEM;
    }
    set_ss_pag(pc, PAG_LOG_INIT_DATA + i, frame);
  }

  // inherit kernel:
  for (int i = 0; i < NUM_PAG_KERNEL; ++i)
  {
    set_ss_pag(pc, i, get_frame(pp, i));
  }
  // inherit code:
  for (int i = PAG_LOG_INIT_CODE; i < PAG_LOG_INIT_CODE + NUM_PAG_CODE; ++i)
  {
    set_ss_pag(pc, i, get_frame(pp, i));
  }
  // alloc Data + Stack:
  for (int i = 0; i < NUM_PAG_DATA; ++i)
  {
    // mapp the pages:
    set_ss_pag(pp, PAG_LOG_INIT_DATA + NUM_PAG_DATA + i, get_frame(pc, PAG_LOG_INIT_DATA + i));
    copy_data((void *)((PAG_LOG_INIT_DATA + i) << 12), (void *)((PAG_LOG_INIT_DATA + NUM_PAG_DATA + i) << 12), PAGE_SIZE);
    del_ss_pag(pp, PAG_LOG_INIT_DATA + NUM_PAG_DATA + i);
  }

  // TLB flush:
  set_cr3(get_DIR(current()));

  // get the new PID:
  c->PID = get_new_pid();
  PID = c->PID;
  // set the quantum:
  c->quantum = p->quantum;

  // make the kernel_ebp to pint to the stack of the child:
  int k_ebp = (int)get_ebp();
  k_ebp = k_ebp - (int)pu + (int)cu;

  // save the content of the ebp of the child(the actual value of ebp that will be restored when the fuction ends):
  // since it's a pointer we need to get it's contents:
  int temp_ebp = *(DWord *)k_ebp;

  *(DWord *)k_ebp = (DWord)&ret_from_fork; // now our ebp is the ret_from_fork.
  k_ebp -= sizeof(DWord);
  *(DWord *)k_ebp = temp_ebp;
  c->kernel_ebp = k_ebp;
  // now we are set for the task switch!

  // finaly we addit to the ready list:
  list_add(&c->list, &readyqueue);

  return PID;
}

void sys_exit()
{
  struct task_struct *t = current();
  page_table_entry *p = get_PT(t);
  /*delete kernel:*/
  for (int i = 0; i < NUM_PAG_KERNEL; ++i)
  {
    del_ss_pag(p, i);
  }
  /*del code:*/
  for (int i = PAG_LOG_INIT_CODE; i < PAG_LOG_INIT_CODE + NUM_PAG_CODE; ++i)
  {
    del_ss_pag(p, i);
  }
  /*del data:*/
  for (int i = PAG_LOG_INIT_DATA; i < PAG_LOG_INIT_DATA + NUM_PAG_DATA; ++i)
  {
    free_frame(get_frame(p, i));
    del_ss_pag(p, i);
  }
  sched_next_rr();
}

int sys_write(int fd, char *buff, int size)
{
  int err = check_fd(fd, ESCRIPTURA);
  if (err < 0)
    return err;
  if (buff == NULL)
    return -EINVAL;
  if (size < 0)
    return -EINVAL;
  char *message = "";
  if (copy_from_user(buff, message, size) == -1)
    return -EINTR;
  return sys_write_console(message, size);
}

unsigned long sys_gettime()
{
  return zeos_ticks;
}