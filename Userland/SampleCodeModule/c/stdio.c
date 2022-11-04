// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>

int read(int fd){
    return sys_read(fd);
}

unsigned long write(int fd, char * str){
    return sys_write(fd, str);
}

int writeChar(int fd, char c){
    return sys_writeChar(fd, c);
}

unsigned char getChar(){
    char c;
    while((c = sys_getChar()) == 0);
    return c;
}

unsigned char getCharOrNull(){
    return sys_getChar();
}

void putCharWithColor(char c, unsigned char colorCode){
    if(c != 0)
        sys_putCharWC(c, colorCode);
}

void putChar(char c){
    if(c != 0)
        sys_putChar(c);
}

void print(char * string){
    sys_print(string);
}

void printWithColor(char * string, unsigned char colorCode){
    sys_printWithColor(string, colorCode);
}



void printCharInPos(char c, int row, int col, unsigned char colorCode){
    sys_printCharInPos(c, row, col, colorCode);
}

void printInPos(char * string, int row, int col, unsigned char colorCode){
    for (int i = 0; string[i] != 0; i++){
        printCharInPos(string[i], row, col + i, colorCode);
    }  
}

void printInt(int num){
	char buffer[40];
	uintToBase(num, buffer, 10);
	print(buffer);
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


int scan(char *buffer, int size){
    unsigned char c;
    int lenght=0;
    while((c = getChar()) != '\n' && lenght < size){
        if(c == BACKSPACE){
            if(lenght > 0)
                buffer[lenght--] = 0;
        } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' ' || c == '/' || c == '-'){
            putChar(c);
            buffer[lenght++] = c;  
        } else if(c == SHIFT){
            sys_saveRegs();
        }     
    }
    while(buffer[lenght-1] == ' '){
        while((c = getChar()) != '\n' && lenght < size){
            if(c == BACKSPACE){
                if(lenght > 0)
                    buffer[lenght--] = 0;
            } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' ' || c == '/' || c == '-'){
                putChar(c);
                buffer[lenght++] = c;  
            } else if(c == SHIFT){
                sys_saveRegs();
            }     
        }
    }
    buffer[lenght] = 0;
    return lenght;
}

void clear(){
    sys_clear();
}

void yield(){
    sys_yield();
}

int getTime(int descriptor){
    return sys_getTime(descriptor);
}

unsigned long getDeciseconds(){
    return sys_getDeciseconds();
}
