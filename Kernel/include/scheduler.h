#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <timerDriver.h>
#include <defs.h>
#include <strings.h>
#include <memManager.h>
#include <IOManager.h>

#define NAME_MAX_SIZE 25
#define PROCESS_STACK_SIZE 0x1000

#define PROCESS_NAME_PRINT_SIZE 12

#define TQ 1 
#define MAX_PRIORITY 0
#define MIN_PRIORITY 19
#define DEFAULT_PRIORITY (MAX_PRIORITY + MIN_PRIORITY + 1)/2

#define VALID_PRIORITY(p) ((p) >= MAX_PRIORITY && (p) <= MIN_PRIORITY)

typedef enum {READY, BLOCKED} states;

typedef enum {BACKGROUND, FOREGROUND} mode;

typedef struct stackFrame{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;   // argv
    uint64_t rdi;   // argc
    uint64_t rbp;
    uint64_t rdx;   // function
    uint64_t rcx;   // pid
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;    // loader
    uint64_t cs;     // 0x8
    uint64_t rflags; // 0x202
    uint64_t rsp;    // rbp
    uint64_t ss;     // 0
} stackFrame;

typedef struct processContext{
    char name[NAME_MAX_SIZE];
    void * allocated[10];
    int argc;
    char ** argv;
    uint64_t pid;
    uint64_t rsp;
    uint64_t rbp;
    uint8_t priority; // Guardamos priority como un byte porque toma valores entre 0 y 20
    int ticketsLeft;
    states state;
    int mode; 
    int fdIn;
    int fdOut;
} processContext;

typedef struct process{
    struct process * next;
    processContext pc;
} process;

typedef struct processList{
    process * first;
    process * last;
    process * iterator;
    int size;
} processList;

extern void timerInterrupt();

extern void _hlt();

int changeProcessState(uint64_t pid, states state);

void createFirstProcess();

uint64_t createProcess(void (*pFunction)(int, char **), int argc, char **argv, int * fd, mode processMode);

void forceExitAfterExec(int argc, char *argv[], void *processFn(int, char **));

uint64_t schedule(uint64_t rsp);

uint64_t getPid();

void initializeScheduler();

uint64_t block(uint64_t pid);

uint64_t unblock(uint64_t pid);

uint64_t kill(uint64_t pid);

int exists(uint64_t pid);

void wait(uint64_t pid);

uint64_t toggleBlocked(uint64_t pid);

void printAllProcessesInfo();

void yield();

uint64_t nice(uint64_t pid, uint64_t newPriority);

void forceTimerTick();

uint64_t setupStack(uint64_t startStack, uint64_t loader, uint64_t argc, uint64_t argv, uint64_t rip);

void initScheduler();

int getFdIn();

int getFdOut();

int getProcessMode();

process * getExecutingP();


#endif //SCHEDULER_H
