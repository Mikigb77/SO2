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
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
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
  int PID=-1;

  // creates the child process

  return PID;
}

void sys_exit()
{
}
int sys_write (int fd, char * buff, int size)
{
  //Check the user permissions:
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

  char buffer [size];
  copy_from_user(buff, buffer, size);
  return sys_write_console(buffer, size);
}

unsigned long sys_gettime(){
  return zeos_ticks;
}
