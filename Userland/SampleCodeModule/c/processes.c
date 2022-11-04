// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>

int isVowel(char c){
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

void sleep(int argSize, char *args[]){
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    int secondsElapsed = sys_getSeconds();
    int final = secondsElapsed + atoull(args[0]);
    while(sys_getSeconds() <= final);
}

void loop(){
    unsigned long long pid = sys_getPid();
    char *loopTime[1] = {"5"};

    while(1){
        sleep(1, loopTime);
        print("Hola!!! Soy el proceso con PID: ");
        printInt(pid);
        putChar('\n');
    }
}

void cat(){
    char c = read(sys_get_fdin());
    while(c != 0){
        char toWrite[] = {c, 0};
        write(sys_get_fdout(), toWrite);
        c = read(sys_get_fdin());
    }
}

void wc(){
    int lines = 0;
    char c = read(sys_get_fdin());
    while (c != 0){
        if (c == '\n')
            lines++;
         c = read(sys_get_fdin());
    }
    print("Lines: ");
    printInt(lines);
    putChar('\n');
}

void filter(){
    char c = read(sys_get_fdin());
    while(c != 0){
        if(!isVowel(c)){
            char toWrite[] = {c, 0};
            write(sys_get_fdout(), toWrite);
        }
        c = read(sys_get_fdin());
    }
    putChar('\n');
}

void testMalloc(){
    sys_malloc(1000);
}
