#ifndef __RTCDriver_H_
#define __RTCDriver_H_

#include <stdint.h>
#include <timerDriver.h>

extern uint8_t getRTC(uint64_t descriptor);

uint8_t getTime(uint64_t descriptor);

uint64_t getDecis();

uint64_t getSeconds();

#endif