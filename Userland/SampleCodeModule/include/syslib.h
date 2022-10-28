#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdint.h>

#define NULL (void*)0

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define EOF -1

typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

int sys_write(int fd, const char *buffer, int size);
int sys_read(int fd,char * buffer,int size);
void sys_time(time_t * time);
int sys_getregs(uint64_t * registers);
void inv_opcode();
void div_zero();
void sys_execve(void (*f1)(), void (*f2)());
void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length);

unsigned int strlen(const char *s);
void excepDivZero();
void excepInvalidOpcode();
int puts(const char* s);
int putChar(char);
char getChar();
int gets(char * s);
unsigned int charBelongs(char *s,char c);
int containsString(const char *p1,const char *p2);
void savePrintMemParams(char *s);
int strcmp (const char *p1, const char *p2);
void getTime();
void printPrime();
void fibonacciNumbs();
void printmem();
void inforeg();
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

extern int sys_getSeconds();
extern int sys_get_fdin();
extern int sys_get_fdout();
extern void sys_sem();
extern void sys_pipe();
extern unsigned long long sys_getPid();
extern int sys_change_process_priority(uint64_t pid, uint8_t newPriority);
extern int sys_change_process_state(uint64_t pid, int state);
extern unsigned long sys_createProcess(void (*pFunction)(int, char **), int argc, char **argv, int * fd, mode processMode);
extern void sys_ps();
extern void sys_nice(unsigned long long pid, char newPriority);
extern int sys_killPs(unsigned long long pid);
extern unsigned long sys_block(uint64_t pid);
extern unsigned long sys_unblock(uint64_t pid);
extern unsigned long sys_sem_open(char *sem_name, uint64_t initialValue);
extern unsigned long sys_sem_wait(char *sem_name);
extern unsigned long sys_sem_post(char *sem_name);
extern unsigned long sys_sem_close(char *sem_name);
extern void sys_exit();
#endif