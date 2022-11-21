#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint8_t RTC(uint8_t mode);

void _timerTick();

int _xchg(int *mutex, int value);

int _xadd(int add, int *num);

char * strcpy(char * destination, const char * source);
void strcat(char * destination, char * source, unsigned int *index);
char* itoa(uint64_t value, char* buffer, int base);

int strlen(char *str);

int atoi(char * str);
int strcmp(char * str1, char * str2);
int iabs(int num);
int atohex(char * str);
char toUpper(char letter);
char toLower(char letter);
void addSpace(char *dest, int* from, int strSize, int numSpace);
//crea un copia de str. Usa malloc para crear el nuevo string, hay que 
//liberarlo con free luego de usarlo
char* strCopy(char* str);

#endif