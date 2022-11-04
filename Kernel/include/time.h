#ifndef _TIME_H_
#define _TIME_H_


/**
 * Cada vez que interrumpe el Timer Tick, se suma un tick
 * al contador
**/
void timer_handler();

/**
 * Devuelve la cantidad de ticks desde que se inicio el sistema
**/
int ticks_elapsed();

/**
 * Devuelve la cantidad de segundos desde que se inicio el sistema
**/
int seconds_elapsed();

/**
 * Devuelve la cantidad de decisegundos desde que se inicio el sistema
**/
int deciseconds_elapsed();

#endif
