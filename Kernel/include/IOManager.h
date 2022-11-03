#ifndef _IO_MANAGER_H
#define _IO_MANAGER_H

#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <pipe.h>
#include <scheduler.h>

#define STDIN 0
#define STDOUT 1

void print(char * str);
void printWithColor(char * str, uint8_t colorCode);
void putChar(char c);
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBase(uint64_t value, uint32_t base);
unsigned char readHandler(int fd);
uint32_t writeStrHandler(int fd, char * str);
uint32_t writeCharHandler(int fd, char c);

#endif