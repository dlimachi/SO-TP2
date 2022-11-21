#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <registers.h>
#include <stdint.h>

void exc_0h(registerStruct * registers);
void exc_6h(registerStruct * registers);
void readError(uint64_t * target);

#endif