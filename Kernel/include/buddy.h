#ifndef _BUDDY_H
#define _BUDDY_H

#include <types.h>
#include <stdint.h>
#include <naiveConsole.h>

void * malloc(size_t request);
void free(void *ptr);
void mem();

#endif