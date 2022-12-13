/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

/** Screen functions **/
/**********************/

Byte inb(unsigned short port);
void printc(char c);

/**
 * @brief prints the char in the specific color
 *
 * @param c char to print
 * @param color color to print ex 0x0200 -> green
 */
void printcc(char c, int color);

void printc_xy(Byte x, Byte y, char c);
void printk(char *string);

#endif /* __IO_H__ */
