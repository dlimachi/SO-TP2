#ifndef __irqDispatcher_H_
#define __irqDispatcher_H_

#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>

/**
 * Rutina para la interrupcion 20 (Timer Tick)
**/
static void int_20();

/**
 * Rutina para la interrupcion 21 (Teclado)
**/
static void int_21();

#endif

