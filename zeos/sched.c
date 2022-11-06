/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include "global.h"
#include "My_ini_settings.h"
#include "schedAsm.h"

/********************************/
/***********Debug****************/

//#include <libc.h>

/********************************/

union task_union task[NR_TASKS]
	__attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry *get_DIR(struct task_struct *t)
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry *get_PT(struct task_struct *t)
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr)) << 12);
}

int allocate_DIR(struct task_struct *t)
{
	int pos;

	pos = ((int)t - (int)task) / sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry *)&dir_pages[pos];

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti"
						 :
						 :
						 : "memory");

	while (1)
	{
		;
	}
}

/**
 * @brief gets the task_struct that contains the list l
 * @param l The pointer to the list contained in the task_struct to get
 * @return a pointer to the task struct that contains l
 */
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
	return list_entry(l, struct task_struct, list);
}

/**
 * @brief returns a new pid for the process
 *
 * @return the new unic PID for the process
 */
int getNewPID()
{
	return (int)++newPID;
}

void init_idle(void)
{
	struct list_head *l = list_first(&freequeue);
	struct task_struct *t = list_head_to_task_struct(l);
	union task_union *tu = (union task_union *)t;
	list_del(l);
	tu->task.PID = 0;
	initParams(t);
	allocate_DIR(&tu->task);
	idle_task = t;
	/**
	 * initialize an execution context for the procees to restore it when it gets assigned the cpu
	 * and executes cpu_idle.
	 */
	unsigned long esp = KERNEL_STACK_SIZE;
	--esp;
	tu->stack[esp] = (unsigned long)&cpu_idle; // value of the @ret
	--esp;
	tu->stack[esp] = 0; // value of kernel_ebp
	tu->task.kernel_ebp = (unsigned int)&tu->stack[esp];
}

void init_task1(void)
{
	struct list_head *l = list_first(&freequeue);
	list_del(l);
	struct task_struct *t = list_head_to_task_struct(l);
	union task_union *tu = (union task_union *)t;

	t->PID = getNewPID();
	initParams(t);
	allocate_DIR(t);
	set_user_pages(t);
	tss.esp0 = (unsigned int)&tu->stack[KERNEL_STACK_SIZE];
	setMSR_ESP0(tss.esp0);
	set_cr3(t->dir_pages_baseAddr);
}

void init_freequeue()
{
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; ++i)
	{
		// we initialize the values
		initParams(&task[i].task);

		// add the task to the list
		list_add_tail(&task[i].task.list, &freequeue);
	}
}

void init_readyqueue()
{
	INIT_LIST_HEAD(&readyqueue);
}

void init_sched()
{
	init_freequeue();
	init_readyqueue();
}

/**
 * @brief get the current task_struct
 * @return Returns a pointer to the current task_struct
 */
struct task_struct *current()
{
	int ret_value;

	__asm__ __volatile__(
		"movl %%esp, %0"
		: "=g"(ret_value));
	return (struct task_struct *)(ret_value & 0xfffff000);
}

void task_switch(union task_union *new)
{
	/**we need to change the stack address*/
	tss.esp0 = &new->stack[KERNEL_STACK_SIZE];
	setMSR_ESP0(tss.esp0);

	struct task_struct *t = current();

	/**
	 * now change the pages_dir and make the TLB flush
	 * (ONLY if the memory between the programs is changed for the user data)
	 */
	if (t->dir_pages_baseAddr != current()->dir_pages_baseAddr)
		set_cr3(current()->dir_pages_baseAddr);

	/**now change the kernel_ebp*/
	task_switch_(new);
}

void initParams(struct task_struct *t)
{
	t->quantum = FULL_QUANTUM;
	t->state = ST_READY;
}

int get_quantum(struct task_struct *t)
{
	return t->quantum;
}

void set_quantum(struct task_struct *t, int new_quantum)
{
	t->quantum = new_quantum;
}
