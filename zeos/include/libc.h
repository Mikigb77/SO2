/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */

#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>

int write(int fd, char *buffer, int size);

void itoa(int a, char *b);

int strlen(char *a);

int getpid();

int fork();

void exit();

int Suma(int a);

void perror();

int gettime();

int get_stats(int pid, struct stats *st);

/*********************************************/
/*********************AUX*********************/

/**
 * @brief prints the buffer
 *
 * @param buff char* to print
 */
void printf(char *buff);

/**
 * @brief prints all the stats
 *
 * @param st pointer to the stats struct to print
 */
void print_stats(struct stats *st);

/**
 * @brief cleans the screen
 *
 */
void clean_screen();
/*********************************************/

#endif /* __LIBC_H__ */
