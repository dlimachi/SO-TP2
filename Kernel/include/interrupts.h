#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <idtLoader.h>
#include <stdint.h>

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _syscallHandler(void);

void _exception0Handler(void);
void _exception6Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picSlaveMask(uint8_t mask);

void picMasterMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

void saveInitialConditions(void* address);

void _sendEOI(void);

uint64_t * createProcessInt(void (*)(),uint64_t *,uint64_t,char **);

void goToProcess(uint64_t *);

#endif /* INTERRUPTS_H_ */
