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
#include <registers.h>
#include <processes.h>
#include <video_driver.h>
#include <time.h>
#include <font.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <memoryManager.h>
#include <lib.h>

int syscallHandler(registerStruct * registers);
void writeStr(registerStruct * registers);

#endif