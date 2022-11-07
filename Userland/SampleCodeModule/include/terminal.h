#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <mystdio.h>
#include <stdio.h>
#include <strings.h>
#include <tests.h>
#include <processes.h>
#include <mysyscall.h>
#include <syscall.h>

#define COMMANDS_COUNT 23
#define BUFFER_SIZE 50

#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9

#define MAX_PRIORITY 0

/**
 * Inicializa la terminal, cargando los comandos y entrando en un ciclo infinito
**/
void startTerminal();

/**
 * Carga los diferentes comandos en tres vectores paralelos mediante un llamados a commandBuilder 
 * que contienen el nombre, la descripcion y la funcion de cada uno
**/
void startCommands();

/**
 * Carga el nombre, la descrición y la función de un comando en su respectivo vector
**/
void commandBuilder(char *name, char *desc, void (*fn)(), int builtin);

/**
 * Ejecuta el comando ingresado, primero lo tokeniza separando por espacios para validar
 * que tenga la cantidad de argumentos correpondiente, si no es asi imprime un mensaje de error al
 * igual que si se ingreso un comando invalido
**/
void executeCommand(char *buffer);

/**
 * Funcion del comando 'help'. Despliega en pantalla todos los comandos disponibles para el usuario 
 * con su respectiva descripción.
**/
void help();

/**
 * Función del comando 'clear'. Limpia la pantalla dejando a la terminal vacía
**/
void clearScreen();

/**
 * Función del comando 'inforeg'. Despliega por pantalla el valor de los registros al momento del llamado
**/
void getRegisters();

/**
 * Función del comando 'printmem'. Realiza un dump de 32 bytes en memoria de una dirección de memoria pasada por parametro
**/
void printmem(int argSize, char *args[]);

/**
 * Función del comando 'time'. Imprime en pantalla la hora y la fecha actual
**/
void printTime();

/**
 * Función del comando 'divzero'. Verifica el correcto funcionamiento de la excepción de división por cero.
 **/
void divZero();

/**
 * Función del comando 'invalidopcode'. Verifica el correcto funcionamiento de código de operación invalido.
**/
void invalidOpCode();

/**
 * Función del comando 'mem'. Imprime en pantalla el estado de la memoria.
**/
void mem();

/**
 * Función del comando 'ps'. Imprime en pantalla una lista con todos los procesos en ejecución.
**/
void ps();

/**
 * Función del comando 'loop'. Crea un proceso que imprime su pid en pantalla cada cierto tiempo.
**/
void runLoop();

/**
 * Función del comando 'block'. Cambia el estado de un proceso entre bloqueado y listo dado su ID.
**/
void block(int argSize, char *args[]);

/**
 * Función del comando 'kill'. Mata un proceso dado su ID.
**/
void kill(int argSize, char *args[]);

/**
 * Función del comando 'nice'. Cambia la prioridad de un proceso.
**/
void nice(int argSize, char *args[]);

/**
 * Función del comando 'sem'. Imprime una lista con todos los semaforos.
**/
void sem();

/**
 * Función del comando 'pipe'. Imprime una lista con todos los semaforos.
**/
void pipe();

/**
 * Función del comando 'testsynchro'
**/
void testSyncWrapper();

/**
 * Función del comando 'testnosynchro'
**/
void testNoSyncWrapper();

/**
 * Función del comando 'cat'. Crea un proceso que imprime en pantalla el input tal como lo recibio.
**/
void catBuitIn(int argSize, char *args[]);

/**
 * Función del comando 'wc'. Crea un proceso que imprime en pantalla la cantidad de lineas del input.
**/
void wcBuitIn(int argSize, char *args[]);

/**
 * Función del comando 'filter'. Crea un proceso que imprime en pantalla la cantidad de vocales del input.
**/
void filterBuitIn(int argSize, char *args[]);

#endif
