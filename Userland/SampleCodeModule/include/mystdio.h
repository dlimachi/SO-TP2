#ifndef _MYSTDIO_H
#define _MYSTDIO_H

#include <stdint.h>
#include <syscall.h>

#define BACKSPACE '\b'
#define SHIFT 201
#define SLASH '/'
#define DASH '-'

// COLORES: El formato es COLORLETRA_COLORFONDO
#define BLACK_BLACK 0x00
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F
#define RED_RED 0x44
#define MAGENTA_MAGENTA 0x55
#define WHITE_WHITE 0xFF
#define LIGHTGREEN_LIGHTGREEN 0xAA
#define BLUE_LIGHTGREEN 0xA1
#define BLACK_LIGHTGREEN 0xA0
#define WHITE_RED 0x4F
#define YELLOW_RED 0x4E
#define BLUE_BLUE 0x11
#define BLUE_GREY 0x71
#define BLUE_WHITE 0xF1
#define GREY_GREY 0x77
#define RED_WHITE 0xF4
#define RED_GREY 0x74

#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0


/**
 * Syscall: Abre un nuevo pipe
**/
extern int * sys_pipeOpen(char * pipeName);

/**
 * Syscall: Cierra un pipe
**/
extern unsigned long sys_pipeClose(char * pipeName);

int read(int fd);
unsigned long write(int fd, char * str);
int writeChar(int fd, char c);

/**
 * Devuelve un caracter ingresado por el usuario siempre que este no
 * sea nulo (distinto de 0)
**/
unsigned char getChar();

/**
 * Devuelve un caracter ingresado por el usuario o en su defecto 0. Se
 * utiliza para que la ejecucion de funciones como la impresion de la hora
 * y el cronometro puedan seguir su ejecucion aunque el usuario no haya
 * ingresado ningun caracter (es decir, cuando sys_getChar devuelve 0)
**/
unsigned char getCharOrNull();

/**
 * Imprime en la posicion actual de la pantalla el string pasado por
 * parametro
**/
void print(char * string);

/**
 * Imprime en pantalla el caracter 'c' con el color predeterminado.
**/
void putChar(char c);

/**
 * Wrapper de sys_putCharWC
**/
void putCharWithColor(char c, unsigned char colorCode);

/**
 * Recibe hasta 'size' caracteres ingresados por el usuario y los guarda en
 * 'buffer' mientras que estos sean distintos que '\n'. Cuando se recibe dicho
 * char, se cierra el string 'buffer' y se retorna la cantidad de caracteres
 * leidos
**/
int scan(char *buffer, int size);

/**
 * Imprime en la posicion actual de la pantalla el string pasado por
 * parametro y en el color indicado por 'colorCode'
**/
void printWithColor(char * string, unsigned char colorCode);

/**
 * Wrapper de sys_clear
**/
void clear();

/**
 * Wrapper de sys_getTime
**/
int getTime(int descriptor);

/**
 * Wrapper de sys_getDeciseconds
**/
unsigned long getDeciseconds();

/**
 * Imprime el string pasado por parametro a partir de la fila 'row' y la columna
 * 'col' de la misma en el color indicado por 'colorCode'
**/
void printInPos(char * string, int row, int col, unsigned char colorCode);

/**
 * Wrapper de sys_printCharInPos
**/
void printCharInPos(char c, int row, int col, unsigned char colorCode);

void printInt(int num);

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
