#ifndef BASH_H
#define BASH_H

#include <stdio.h>
#include <strings.h>
#include <tests.h>
#include <processes.h>
#include <syslib.h>

#define COMMANDS_COUNT 23
#define BUFFER_SIZE 50

#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9

#define MAX_PRIORITY 0

void startTerminal();

void startCommands();

void commandBuilder(char *name, char *desc, void (*fn)(), int builtin);

void executeCommand(char *buffer);

void help();

void clearScreen();

void getRegisters();

void printmem(int argSize, char *args[]);

void printTime();

void divZero();

void invalidOpCode();

void mem();

void runLoop();

void kill(int argSize, char *args[]);

void sem();

void catBuitIn(int argSize, char *args[]);

void wcBuitIn(int argSize, char *args[]);

void filterBuitIn(int argSize, char *args[]);

#endif
