// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <IOManager.h>

void print(char * str){
    writeStrHandler(getFdOut(), str);
}

void printWithColor(char * str, uint8_t colorCode){
    writeStrHandler(getFdOut(), str, colorCode);
}

void putChar(char c){
    writeCharHandler(getFdOut(), c);
}

void printDec(uint64_t value){
	printBase(value, 10);
}

void printHex(uint64_t value){
	printBase(value, 16);
}

void printBase(uint64_t value, uint32_t base){
    char buffer[64] = {'0'};
    uintToBase(value, buffer, base);
    print(buffer);
}

unsigned char readHandler(int fd){
    if(fd < 0)
        return 0;
    if(fd == STDIN)
        return kb_getChar();
    return readPipeWithFd(fd);  
}

uint32_t writeStrHandler(int fd, char * str){
    if(fd < 0)
        return 0;
    if(fd == STDOUT){
        ncPrint(str);
        return strlen(str);
    }
    else
        return writeInPipeWithFd(fd, str);
}

uint32_t writeCharHandler(int fd, char c){
    if(fd < 0)
        return 0;
    if(fd == STDOUT){
        ncPrint(c);
        return 1;
    }
    else
        return writeCharInPipeWithFd(fd, c);
}