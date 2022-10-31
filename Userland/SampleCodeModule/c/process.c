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

void loop(uint64_t argc, char *argv[]){
    if (argc < 2){
        print("Es necesario ingresar el tiempo de loop\n");
        sys_exit();
        return;
    }

    int64_t delay = satoi(argv[1]);

    while (1){
        sleep(1, (uint64_t)delay);
        print("Hola!!! Soy el proceso con PID: ");
        printNum(sys_pid());
        putChar('\n');
    }
    sys_exit();
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

void ps(uint64_t argc, char *argv[]){
    sys_ps();
    sys_exit();
}

void pipe(uint64_t argc, char *argv[]){
    sys_printPipes();
    sys_exit();
}

void block(uint64_t argc, char *argv[]){
    if (argc == 1){
        print("The pid number is needed as parameter");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t)satoi(argv[1]);

    sys_block(pid);
    sys_exit();
}

void nice(uint64_t argc, char *argv[]){
    if (argc <= 2){
        print("Es necesario el pid del proceso");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t)satoi(argv[1]);
    uint32_t priority = (uint32_t)satoi(argv[2]);

    sys_change_process_priority(pid, priority);
    sys_exit();
}