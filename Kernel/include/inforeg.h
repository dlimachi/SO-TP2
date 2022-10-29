#ifndef INFOREG_H
#define INFOREG_h

#include <stdint.h>
#include <defs.h>

void setRegisters(uint64_t * registers);

void printMemory(uint64_t argument);

int getRegisters(uint64_t * output); 

#endif
