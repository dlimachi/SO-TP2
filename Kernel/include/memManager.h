// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef _MEMMANAGER_H
#define _MEMMANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

struct memoryInfo {
    size_t free;
    size_t occupied;
    size_t totalSize;
};

void *memAlloc(size_t nbytes);

void memFree(void *ap);

void createMemoryManager(void *managedMemory, size_t size);

void memoryInfo(struct memoryInfo *info);

void mem();

#endif
