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

#include <entry.h>

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
  if (list_empty(&freequeue))
    return -MYENFRETSK;

  /*first get the parent tu (curr)*/
  struct task_struct *t = current();
  union task_union *tu = (union task_union *)t;

  /*now we get the son*/
  struct list_head *l = list_first(&freequeue);
  struct task_struct *t1 = list_head_to_task_struct(l);
  union task_union *tu1 = (union task_union *)t1;
  /*rease the task from free*/
  list_del(l);

  /*first get a PID for the son*/
  PID = getNewPID();

  /*copy the task so it's the same as the father*/
  *tu1 = *tu;
  /*we also assign the new PID*/
  t1->PID = PID;
  /*we can set the kernel_ebp, since the 'tu' allready have the stack[] filled with a return @ and the ebp*/
  // t1->kernel_ebp = &tu1->stack[KERNEL_STACK_SIZE - 2];
  //  |
  //  V
  /**
   * this is usless since we need to change it again since we need to make the restore all
   * and return from the sysenter we can doit with a function (ret_from_fork) but we need to chenge the
   * kernel_esp for that (done below)
   */

  /*now we need to allocate the frames to store the user data*/
  int frames[NUM_PAG_DATA];

  for (int i = 0; i < NUM_PAG_DATA; ++i)
  {
    frames[i] = alloc_frame();
    if (frames[i] == -1)
    {
      for (; i >= 0; --i)
        free_frame(frames[i]);
      list_add_tail(l, &freequeue);
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

  /**
   * creem la entrada de la PT pel pare que despres eliminarem només per a la traducció.
   * li hem de posar al pare pq les adrecess logiques del pare i fill estan compartides de manera que
   * en el TLB si intentem moure de la pag log 8(exemple) a la pag logica 8 del fill, seria un hit amb la del pare i pertant
   * estariem fent copies del frame del pare al mateix frame.
   *
   * per evitar'ho hem de crear unaltre entrada a la taula del pare que mapegi (que en el TLB surti pag-frame)
   * el frame en el que volem escriure del fill (entrada TLB: Parent-new-page --> son-frame).
   */
  for (int i = 0; i < NUM_PAG_DATA; ++i)
  {
    /*pag for the parent pointing to the frame to copy*/
    set_ss_pag(p, FIRST_FREE_PAG + i, frames[i]);

    /*now we need to make the copy*/
    copy_data((void *)((NUM_PAG_KERNEL + NUM_PAG_CODE + i) << 12), (void *)((FIRST_FREE_PAG + i) << 12), PAGE_SIZE);

    /*now we must set the PT entry for the son linking the PT entry to the frame where we copyed the info*/
    set_ss_pag(p1, (NUM_PAG_KERNEL + NUM_PAG_CODE + i), frames[i]);
  }

  /*finaly we need to delete all the entris added to the parent*/
  for (int i = 0; i < NUM_PAG_DATA; ++i)
    del_ss_pag(p, FIRST_FREE_PAG + i);

  /*last is make a flush of the TLB*/
  set_cr3(get_DIR(t));

  /*now we need to prepare to restore the hw context because in our stack we have all the pushes from SAVE_ALL since it is a syscall*/
  /*first we need to change the pointer of KERNEL_EBP to the top of the stack in wich we will add the return code (with the restore all)*/
  /*we also need an ebp for the inner task switch to pop in the restoring process*/
  tu->stack[KERNEL_STACK_SIZE - 17] = &ret_from_fork; // the return 0 for the child is done here
  t->kernel_ebp = &tu->stack[KERNEL_STACK_SIZE - 18];
  tu->stack[KERNEL_STACK_SIZE - 18] = 0; // if there is some error this will produce an exception

  /*finaly we add the process to the readyqueue*/
  list_add_tail(&t->list, &readyqueue);

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
