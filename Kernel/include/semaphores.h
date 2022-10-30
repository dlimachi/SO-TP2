#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_

#include <stdint.h>
#include <stddef.h>
#include <scheduler.h>
#include <inforeg.h>
#include <memManager.h>
#include <strings.h>
#include <naiveConsole.h>
#include <IOManager.h>

#define NAME_MAX_SIZE 25
#define FAILED 0
#define SUCCESS 1

typedef struct pNode{
    struct pNode * next;
    uint64_t pid;
} pNode;

typedef struct TSem {
    struct TSem * next;
    char name[NAME_MAX_SIZE];
    uint64_t lock;
    int value;
    // lista de procesos que tiene en un wait
    pNode * firstProcess;
    pNode * lastProcess;
    int waitingProcesses;
    int openedBy;
} TSem;

typedef struct semList{
    TSem * first;
    TSem * last;
    int size;
} semList;

extern uint64_t _xchgLock(uint64_t * lock);

extern uint64_t _unlock(uint64_t * lock);

uint64_t semOpen(char *name, int initialValue);

uint64_t semClose(char * semName);

uint64_t semWait(char * semName);

uint64_t semPost(char * semName);

void printListofSemaphores();

void initSemaphores();

#endif