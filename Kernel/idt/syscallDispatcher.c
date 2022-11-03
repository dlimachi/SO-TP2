// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscallDispatcher.h>
#include <scheduler.h>

static uint64_t sys_read(unsigned int fd,char* output, uint64_t count);
static void sys_write(unsigned fd,const char* buffer, uint64_t count);
static void sys_time(time_t * s);
static void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length);

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9, uint64_t * registers) {
	switch (rdi) {																	
		case 0:
			return readHandler(STDIN); 													
		case 1:
			putChar((char) rsi); 					
			return 1;																
		case 2:
			return getTime(rsi);													
		case 3:
			ncClear();																
			return 3;	
		case 4:
			saveRegisterInfo(); 														
			return 4;
		case 5:
			printMemory(rsi);														
			return 5;
		case 6:
			ncPrintCharInPos((char)rsi, (int)rdx, (int)rcx, (unsigned char)r8);		
			return 6;
		case 7:
			return getDecis();													
		case 8:
			getRegistersInfo(); 												
			return 8;
		case 9:
			printAllProcessesInfo();
			return 9;
		case 10:
			return getPid();
		case 11:
			return toggleBlocked(rsi);												
		case 12:
			return kill(rsi);												        
		case 13:
			return getSeconds();
		case 14:
			mem();
			return 14;
		case 15:
			return createProcess((void (*)(int, char **))rsi, (int)rdx, (char **)rcx, (int *) r8, (mode) r9);
		case 16:
			printListofSemaphores();
			return 16;
		case 17:
			nice((pid_t)rsi, (uint8_t)rdx);
			return 17;
		case 18:
			yield();
			return 18;
		case 19:
			return (uint64_t)malloc(rsi);
		case 20:
			free((void *)rsi);
			return 20;
		case 21:
			return semOpen((char *)rsi, (int)rdx);
		case 22:
			return semClose((char *)rsi);
		case 23:
			return semWait((char *)rsi);
		case 24:
			return semPost((char *)rsi);
		case 25:
			return block((pid_t)rsi);
		case 26:
			return unblock((pid_t)rsi);
		case 27:
			printListOfPipes();
			return 27;
		case 28:
			wait((pid_t)rsi);
			return 28;
		case 29:
			return exists((pid_t)rsi);
		case 30:
			return (uint64_t)pipeOpen((char *)rsi);
		case 31:
			return pipeClose((char *)rsi);
		case 32:
			return getFdIn();
		case 33:
			return getFdOut();
		case 34:
			return readHandler((int)rsi);
		case 35:
			return writeStrHandler((int) rsi, (char *) rdx);
		case 36:
			putChar((char) rsi);
			return 36;
		case 37:
			print((char *)rsi);
			return 37;
		case 38:
			printWithColor((char *)rsi, (uint8_t)rdx);
			return 38;
		case 39:
			return writeCharHandler((int) rsi, (char) rdx);
		case 40:
			return getProcessMode();
	}
	return 0;
}


static uint64_t sys_read(unsigned int fd,char* output, uint64_t count){
    switch (fd)
    {
    case STDIN:
        return readBuffer(output, count);
        break;
    
    default:
        return 0;
    }
}

static void sys_write(unsigned fd,const char* buffer, uint64_t count){
    uint64_t i = 0;
    while (i < count)
    {
        switch(fd){
            case STDOUT:
                ncPrintChar(buffer[i]);
                break;
            case STDERR:
                ncPrintCharFormat(buffer[i], ERROR_FORMAT);
                break;
            default:
                return;
        }
        i++;
    }
}

static void sys_time(time_t * s){
    s->day = localDay();
    s->month = localMonth();
    s->year = localYear();
    s->hours = localHours();
    s->minutes = getMinutes();
    s->seconds = getSeconds();
}

static void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length){
    memcpy((void*)buffer, (void*)address, length);
}
