#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <naiveConsole.h>
#include <stdint.h>
#include <memory.h>
#include <ioManager.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
#define IP_INDEX 15
#define SP_INDEX 18

/**
 * Devuelve el valor del registro SP (A donde apunta el Stack)
**/
extern uint64_t getSP();

/**
 * Guarda el valor de cada registro en un buffer que despues
 * imprime
**/
extern void saveRegisterInfo();

/**
 * Deriva la excepcion (cuyo numero se indica en 'exception') a la funcion
 * adecuada e imprime el estado de todos los registros
**/
void exceptionDispatcher(int exception, uint64_t *registers);

/**
 * Handler de la excepcion de division por cero (imprime en pantalla)
**/
void zeroDivision();

/**
 * Handler de la excepcion de operando invalido (imprime en pantalla)
**/
void invalidOpcode();

/**
 * Guarda el estado inicial del sistema (A donde apuntan IP y SP)
 * para volver al mismo en caso de excepcion
**/
void saveInitialState(uint64_t IP, uint64_t SP);


/**
 * Reinicia la terminal
**/
void restartTerminal(uint64_t *registers);

typedef struct initialState{
    uint64_t IP;
    uint64_t SP;
}initialState;

#endif