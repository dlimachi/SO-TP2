#ifndef _SYSCALL_H
#define _SYSCALL_H

typedef enum {BACKGROUND, FOREGROUND} mode;

extern char sys_getChar();

extern void sys_putCharWC(char c, unsigned char colorCode);

extern int sys_getTime(int descriptor);

extern void sys_getRegistersInfo();

extern void sys_clear();

extern void sys_PrintMem(char * address);

extern void sys_printCharInPos(char c, int row, int col, unsigned char colorCode);

extern unsigned long sys_getDeciseconds();

extern void sys_raiseInvOpCodeExc();

extern void sys_saveRegs();

extern void sys_mem();

extern void sys_ps();

extern int sys_togglePsState(unsigned long long pid);

extern int sys_getSeconds();

extern void sys_sem();

extern void sys_pipe();

extern unsigned long long sys_getPid();

extern unsigned long long sys_createProcess(void (*pFunction)(int, char **), int argc, char **argv, int * fd, mode processMode);

extern void sys_nice(unsigned long long pid, char newPriority);

extern int sys_killPs(unsigned long long pid);

extern unsigned long sys_block(uint64_t pid);

extern unsigned long sys_unblock(uint64_t pid);

extern unsigned long sys_sem_open(char *sem_name, uint64_t initialValue);

extern unsigned long sys_sem_wait(char *sem_name);

extern unsigned long sys_sem_post(char *sem_name);

extern unsigned long sys_sem_close(char *sem_name);

extern void sys_yield();

extern void * sys_malloc(uint32_t size);

extern void sys_free(void *pr);

extern void sys_wait(unsigned long long pid);

extern int sys_exists(unsigned long long pid);

extern int sys_get_fdin();

extern int sys_get_fdout();

extern int sys_read(int fd);

extern unsigned long sys_write(int fd, char * str);

extern void sys_putChar(char c);

extern void sys_print(char * str);

extern void sys_printWithColor(char * str, char colorCode);

extern int sys_writeChar(int fd, char c);

extern int sys_getProcessMode();

#endif