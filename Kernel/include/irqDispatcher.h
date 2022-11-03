#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H

#include <keyboardDriver.h>
#include <timerDriver.h>
#include <stdint.h>

void irqDispatcher(uint64_t irq, uint64_t * registers);
static void int_20();
static void int_21(uint64_t * registers);
#endif