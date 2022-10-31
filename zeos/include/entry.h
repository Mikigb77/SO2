/*
 * entry.h - Definició del punt d'entrada de les crides al sistema
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__
void keyboard_handler();
void clock_handler();
int system_call_handler();
int syscall_handler_sysenter();
int ret_from_fork();
#endif /* __ENTRY_H__ */
