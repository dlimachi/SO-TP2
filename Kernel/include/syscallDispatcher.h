#ifndef SYSCALL_DISPATCHER_H
#define SYSCALL_DISPATCHER_H

#include <stdint.h>
#include <keyboardDriver.h>
#include <rtcDriver.h>
#include <memory.h>
#include <naiveConsole.h>
#include <scheduler.h>
#include <semaphores.h>
#include <pipes.h>
#include <IOManager.h>

extern void saveRegisterInfo();

/**
 * Deriva el numero de syscall (en rdi) a la rutina correspondiente y le transfiere sus parametros 
**/
uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif