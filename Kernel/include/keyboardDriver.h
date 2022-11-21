#ifndef __keyboardDriver_H_
#define __keyboardDriver_H_

#include <naiveConsole.h>
#include <registers.h>
#include <stdint.h>

#define CTRL_L 29
#define SHIFT_L 42
#define SHIFT_R 54

#define BLOCK_M 58

#define KEY_RELEASED 128
#define KEYS 59

#define F1 59
#define F12 88

void keyboardHandler(registerStruct *);
void readKeyboard(char * buff, uint64_t size, uint64_t * count);
void bufferEmpty(uint64_t * target);
void isMayus(uint64_t * target);
void initializeFunctionKeys();
void setFunctionKeyMethod(uint64_t index, void (*function)());


#endif