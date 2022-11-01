/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>
#include "global.h"

#define NR_TASKS 10
#define KERNEL_STACK_SIZE 1024

enum state_t
{
  ST_RUN,
  ST_READY,
  ST_BLOCKED
};

struct task_struct
{
  int PID; /* Process ID. This MUST be the first field of the struct. */
  /*pointer that points to the base dir of the stack, usually stack[KERNEL_STACK_SIZE - 2]*/
  /*DO NOT CHANGE IT'S POSITION (2nd place of the struct)*/
  unsigned int kernel_ebp;
  int quantum;
  page_table_entry *dir_pages_baseAddr;
  struct list_head list; /*list that serves as ancor to get the process in the freequeue, ready, etc*/
};

union task_union
{
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE]; /* pila de sistema, per procés */
};

extern union task_union task[NR_TASKS]; /* Vector de tasques */

#define KERNEL_ESP(t) (DWord) & (t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP KERNEL_ESP(&task[1])

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

struct task_struct *current();

void task_switch(union task_union *new);

struct task_struct *list_head_to_task_struct(struct list_head *l);

int allocate_DIR(struct task_struct *t);

page_table_entry *get_PT(struct task_struct *t);

page_table_entry *get_DIR(struct task_struct *t);

/* Headers for the scheduling policy */
void sched_next_rr();
void update_process_state_rr(struct task_struct *t, struct list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();

/***********************************************/
/*******************My Functions****************/

/**
 * @brief adds 1 to the last pid created and returns it
 * @return last PID created + 1
 */
int getNewPID();

/**
 * @brief initializes the parameters for a task_struct t
 * @param t pointer to the task struct to initialize
 */
void initParams(struct task_struct *t);

/**
 * @brief returns the quantum of a process
 * @param t pointer to the task_struct to get the quantum from
 * @return the quantum of the process
 */
int get_quantum(struct task_struct *t);

/**
 * @brief sets the quantum of a task
 * @param t pointer to the target task to set the quantum
 * @param new_quantum the new value of the quantum
 */
void set_quantum(struct task_struct *t, int new_quantum);

/***********************************************/

#endif /* __SCHED_H__ */
