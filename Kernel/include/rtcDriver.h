#ifndef __RTCDriver_H_
#define __RTCDriver_H_

#include <stdint.h>
#include <time.h>

extern uint8_t getRTC(uint64_t descriptor);

/**
 * Devuelve el campo de la fecha/hora especificada en 'descriptor'
**/
uint8_t getTime(uint64_t descriptor);

/**
 * Devuelve el tiempo desde que se inicio el sistema en decisegundos
**/
uint64_t getDecis();

/**
 * Devuelve el tiempo desde que se inicio el sistema en segundos
**/
uint64_t getSeconds();

#endif