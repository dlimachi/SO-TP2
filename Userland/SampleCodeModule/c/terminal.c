// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <terminal.h>

static char * commandsNames[COMMANDS_COUNT];
static char * commandsDesc[COMMANDS_COUNT];
static void (*commandsFn[COMMANDS_COUNT])(int argSize, char *args[]);
static int commandsBuiltIn[COMMANDS_COUNT];

static int commandIndex = 0;

void startTerminal(){
    startCommands();
    sys_nice(sys_getPid(), MAX_PRIORITY);
    while(1){
        char buffer[BUFFER_SIZE] = {0};
        printWithColor("$> ", GREEN_BLACK);
        scan(buffer, BUFFER_SIZE);
        putChar('\n');
        executeCommand(buffer);
    }
    
}

void startCommands(){
    commandBuilder("help", "Displays information about every command available.", &help, TRUE);
    commandBuilder("clear", "Clears the screen.", &clearScreen, TRUE);
    commandBuilder("printmem", "Displays a 32 bytes memory dump of the address passed as an argument", &printmem, TRUE);
    commandBuilder("time", "Displays the current time and date.", &printTime, TRUE);
    // commandBuilder("inforeg", "Displays the information of all the registers, if saved before.", &getRegisters, TRUE);
    //commandBuilder("divzero", "Displays exception of division by zero.", &divZero, TRUE);
    // commandBuilder("invalidopcode", "Displays exception of an invalid operation code.", &invalidOpCode, TRUE);
    commandBuilder("phylo", "Philosophers problem", &phylo_main, FALSE);
    commandBuilder("mem", "Displays the current memory state.", &mem, TRUE);
    commandBuilder("ps", "Displays a list with all running processes.", &ps, TRUE);
    commandBuilder("sleep", "Delay for a specified amount of time.", &sleep, TRUE);
    commandBuilder("loop", "Displays current PID with a message.", &loop, FALSE);
    commandBuilder("nice", "Changes a process priority.", &nice, TRUE);
    commandBuilder("block", "Blocks a running process given its pid.", &block, TRUE);
    commandBuilder("kill", "Kills a running process given its pid.", &kill, TRUE);
    commandBuilder("sem", "Displays a list with all the semaphores.", &sem, TRUE);
    commandBuilder("testmm", "Test for the memory manager.", &test_mm, FALSE);
    commandBuilder("testprocesses", "Test for the creation of processes.", &test_processes, FALSE);
    commandBuilder("testpriority", "Test for the priority of the scheduler.", &test_prio, FALSE);
    commandBuilder("testsynchro", "Test for the synchronization of processes (with SEM).", &testSyncWrapper, FALSE);
    commandBuilder("testnosynchro", "Test for the synchronization of processes (no SEM).", &testNoSyncWrapper, FALSE);
    commandBuilder("cat", "Displays the input on the screen.", &cat, FALSE);
    commandBuilder("wc", "Displays the quantity of lines of the input.", &wc, FALSE);
    commandBuilder("filter", "Filters the vowels of the input.", &filter, FALSE);
    commandBuilder("pipe", "Displays a list with all the pipes.", &pipe, TRUE);
    commandBuilder("test", "Test malloc without free.", &testMalloc, FALSE);
}

void commandBuilder(char *name, char *desc, void (*fn)(), int builtin){
    commandsNames[commandIndex] = name;
    commandsDesc[commandIndex] = desc;
    commandsFn[commandIndex] = fn;
    commandsBuiltIn[commandIndex] = builtin;
    commandIndex++;
}

void executeCommand(char *buffer){
    char *arguments[5];
    int argumentsCount = strtok(buffer, ' ', arguments, 5);

    if(argumentsCount <= 0 || argumentsCount > 5){
        print("Invalid amount of arguments.\n");
        return;
    }

    for(int i=0; i< COMMANDS_COUNT; i++){
        if(strcmp(arguments[0], commandsNames[i]) == 0){
            
            if(commandsBuiltIn[i] && argumentsCount == 1){
                (*commandsFn[i])(argumentsCount - 1, arguments + 1);
                return;

            } else {
                mode processMode = FOREGROUND;

                if(argumentsCount == 1){
                    if(strcmp(arguments[0], "cat") == 0){
                        (*catBuitIn)(argumentsCount - 1, arguments + 1);
                        return;
                    }
                    if(strcmp(arguments[0], "wc") == 0){
                        (*wcBuitIn)(argumentsCount - 1, arguments + 1);
                        return;
                    }
                    if(strcmp(arguments[0], "filter") == 0){
                        (*filterBuitIn)(argumentsCount - 1, arguments + 1);
                        return;
                    }
                }

                if(argumentsCount == 2 && arguments[1][0] == '-')
                    processMode = BACKGROUND;
                
                if(argumentsCount == 3 && arguments[1][0] == '/'){ 
                    for(int j=0; j< COMMANDS_COUNT; j++){
                        if(strcmp(arguments[2], commandsNames[j]) == 0){
                            int * pipeFds = sys_pipeOpen("pipe");

                            if(pipeFds == NULL){
                                print("Pipe opening error\n");
                                return;
                            }

                            int fdsP1[2] = {0, pipeFds[0]};
                            int fdsP2[2] = {pipeFds[1], 1};

                            char *argv[] = {commandsNames[i]};
                            int pidP1 = sys_createProcess(commandsFn[i], 1, argv, fdsP1, FOREGROUND);
                            argv[0] = commandsNames[j];
                            int pidP2 = sys_createProcess(commandsFn[j], 1, argv, fdsP2, FOREGROUND);
                            sys_wait(pidP1);
                            sys_wait(pidP2);
                            sys_pipeClose("pipe");
                        }
                    }         
                } else {
                    int pid = sys_createProcess(commandsFn[i], 1, arguments, NULL, processMode);
                    if(processMode == FOREGROUND){
                        sys_wait(pid);
                    }
                }
                return;
            }
        }
    }
    print("Invalid command.\n");
}

void help(){
    for(int i=0; i < COMMANDS_COUNT; i++){
        printWithColor(commandsNames[i], YELLOW_BLACK);
        print(" - ");
        print(commandsDesc[i]);
        putChar('\n');
    }
}

void clearScreen(){
    clear();
}

void getRegisters(){
    sys_getRegistersInfo();
}

void printmem(int argSize, char *args[]){
    if(argSize != 1){
        print("Invalid amount of arguments.\n");
        return;
    }
    unsigned long address = hexaStringToInt(args[0]);
    sys_PrintMem((char *) address);
}

void printTime(){
    char str[3];
    int hours = sys_getTime(HOURS);

    itoa(hours, str);
    print(str);

    print(":");

    itoa(sys_getTime(MINUTES), str);
    print(str);

    print(" - ");

    itoa(sys_getTime(DAY), str);
    print(str);

    print("/");

    itoa(sys_getTime(MONTH), str);
    print(str);

    print("/");

    itoa(sys_getTime(YEAR), str);
    print(str);

    putChar('\n');
}

void divZero(){
    // int x = 1/0;
    // if(x){
    // }
}

void invalidOpCode(){
    sys_raiseInvOpCodeExc();
}

void mem(){
    sys_mem();
}

void ps(){
    sys_ps();
}

void runLoop(){
    char *argv[] = {"loop"};
	sys_createProcess(&loop, 1, argv, NULL, FOREGROUND);
}

void block(int argSize, char *args[]) {
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    
    unsigned long long pid = atoull(args[0]);
    
    if (sys_togglePsState(pid) == 0) {
        print("The process does not exist.\n");
    } else {
        print("Success!\n");
    }
}

void kill(int argSize, char *args[]) {
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    
    unsigned long long pid = atoull(args[0]);
    
    if (sys_killPs(pid) == 0) {
        print("The process does not exist.\n");
    } else {
        print("Success!\n");
    }
}

void nice(int argSize, char *args[]){
    if (argSize != 2) {
        print("Invalid amount of arguments.\n");
        return;
    }

    unsigned long long pid = atoull(args[0]);
    char newPriority = atoull(args[1]);

    sys_nice(pid, newPriority);
}
void sem(){
    sys_sem();
}

void pipe(){
    sys_pipe();
}

void testSyncWrapper(){
    test_sync(1);
}

void testNoSyncWrapper(){
    test_sync(0);
}

void catBuitIn(int argSize, char *args[]){
    char c;
    char buffer[BUFFER_SIZE];
    int lenght = 0;
    while((c = getChar()) != '\n' && lenght < BUFFER_SIZE){
        if(c == BACKSPACE){
            if(lenght > 0)
                buffer[lenght--] = 0;
        } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' ' || c == '/' || c == '-'){
            putChar(c);
            buffer[lenght++] = c;  
        }    
    }
    buffer[lenght] = 0;
    putChar('\n');
    print(buffer);
    putChar('\n');
}

void wcBuitIn(int argSize, char *args[]){
    int lines = 0;
    char c;
    while((c = getChar()) != '\n'){
        if (c == '\n')
            lines++;
        
        if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' ' || c == '/' || c == '-')
            putChar(c);     
    }
    putChar('\n');
    print("Lines: ");
    printInt(lines);
    putChar('\n');
}

void filterBuitIn(int argSize, char *args[]){
    char c;
    char buffer[BUFFER_SIZE];
    int lenght = 0;
    while((c = getChar()) != '\n' && lenght < BUFFER_SIZE){
        if(c == BACKSPACE){
            if(lenght > 0)
                buffer[lenght--] = 0;
        } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' ' || c == '/' || c == '-'){
            putChar(c);

            if(!isVowel(c))
                buffer[lenght++] = c; 
        }
    }
    buffer[lenght] = 0;
    putChar('\n');
    print(buffer);
    putChar('\n');
}