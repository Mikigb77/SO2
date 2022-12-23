/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <list.h>
#include "global.h"
#include "kernelUtils.h"

union task_union task[NR_TASKS]
	__attribute__((__section__(".data.task")));

struct task_struct *list_head_to_task_struct(struct list_head *l)
{
	return list_entry(l, struct task_struct, list);
}

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

void init_idle(void)
{
	struct list_head *l = list_first(&freequeue);
	list_del(l);
	struct task_struct *t = list_head_to_task_struct(l);
	union task_union *tu = (union task_union *)t;
	t->PID = 0;
	allocate_DIR(t);
	tu->stack[KERNEL_STACK_SIZE - 1] = (unsigned long)&cpu_idle;
	tu->stack[KERNEL_STACK_SIZE - 2] = 0;
	t->kernel_ebp = (int)&(tu->stack[KERNEL_STACK_SIZE - 2]);
	idle_task = t;
}

void init_task1(void)
{
	struct list_head *l = list_first(&freequeue);
	list_del(l);
	struct task_struct *t = list_head_to_task_struct(l);
	union task_union *tu = (union task_union *)t;
	t->PID = get_new_pid();
	allocate_DIR(t);
	set_user_pages(t);
	tss.esp0 = (DWord) & (tu->stack[KERNEL_STACK_SIZE]);
	setMSR(0x175, (unsigned long)&(tu->stack[KERNEL_STACK_SIZE]));
	set_cr3(t->dir_pages_baseAddr);
}

void init_queues()
{
	// freequeue:
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; ++i)
	{
		struct task_struct *t = &task[i].task;
		list_add(&t->list, &freequeue);
	}
	// readyqueue:
	INIT_LIST_HEAD(&readyqueue);
}

void init_sched()
{
	init_queues();
}

struct task_struct *current()
{
	int ret_value;

	__asm__ __volatile__(
		"movl %%esp, %0"
		: "=g"(ret_value));
	return (struct task_struct *)(ret_value & 0xfffff000);
}

/* CALLED FROM THE TASK_SWITCH IN kernelUtils.S */
void inner_task_switch(union task_union *new)
{

	page_table_entry *pt_new = get_DIR(&new->task);

	/* now the context switch */
	// update system stack:
	tss.esp0 = (int)&(new->stack[KERNEL_STACK_SIZE]);
	setMSR(0x175, (unsigned long)&(new->stack[KERNEL_STACK_SIZE]));

	// update the page table pointed by reg cr3:
	set_cr3(pt_new);

	// change the stack:
	switch_stack(&current()->kernel_ebp, new->task.kernel_ebp);
}

int get_quantum(struct task_struct *t)
{
	return t->quantum;
}
void set_quantum(struct task_struct *t, int new_quantum)
{
	t->quantum = new_quantum;
}

void update_sched_data_rr()
{
	--quantum;
}

int needs_sched_rr()
{
	if (quantum <= 0)
		return true;
	return false;
}

void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue)
{
}

void sched_next_rr()
{
}

void schedule()
{
	update_sched_data_rr();
	if (needs_sched_rr())
		sched_next_rr();
}