#ifndef MEMORY_DRIVER_H
#define MEMORY_DRIVER_H

#include <stdint.h>
#include <video_driver.h>
#include <stddef.h>
#include <memoryManager.h>

#define HEAP_SIZE (1024*1024*64)

void* sbrk(uint64_t size);
void sbrSyscall(uint64_t size, void** result);

void* getCurrentMemoryLimit();
void* getStartMemory();

#endif