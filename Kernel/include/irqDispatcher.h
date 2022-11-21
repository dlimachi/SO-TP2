#ifndef __irqDispatcher_H_
#define __irqDispatcher_H_

#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <registers.h>

/**
 * Rutina para la interrupcion 20 (Timer Tick)
**/
static void int_20();

/**
 * Rutina para la interrupcion 21 (Teclado)
**/
static void int_21(registerStruct * registers);

static void int_80h(registerStruct * registers);

#endif

