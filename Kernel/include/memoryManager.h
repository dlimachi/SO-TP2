#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <types.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <ioManager.h>
#include <scheduler.h>

void * malloc(size_t wantedSize);
void free(void * p);
void mem();

#endif