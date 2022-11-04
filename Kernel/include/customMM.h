#ifndef _CUSTOM_MM_H_
#define _CUSTOM_MM_H_

#include <types.h>
#include <stdint.h>
#include <naiveConsole.h>

void * malloc(size_t wantedSize);
void free(void * p);
void mem();

#endif