#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H
#include <stdint.h>

/**
 * Cada vez que interrumpe el Timer Tick, se suma un tick
 * al contador
**/
void timeHandler();

/**
 * Devuelve la cantidad de ticks desde que se inicio el sistema
**/
uint64_t getTicks();

/**
 * Devuelve la cantidad de segundos desde que se inicio el sistema
**/
int secondsElapsed();

/**
 * Devuelve la cantidad de decisegundos desde que se inicio el sistema
**/
int deciseconds_elapsed();


#endif

