// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>
#include <memManager.h>
#include <lib.h>

#define MAXFD 5
#define QUANTUM 1
#define STACK_SIZE 4096 // 4Kb
#define DEFAULT_PRIORITY 1
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define FIRST_PID 1

typedef enum {
    READY,
    BLOCKED,
    KILLED,
} State;

typedef struct PCB {
    uint64_t rsp;
    State state;
    uint8_t ppid;
    uint8_t pid;
    uint64_t fd[MAXFD];
    uint64_t priority;
    uint64_t remainingCPUTime;
    void *stackMem;
    char **argv;
    int argc;
} pcb_t;

typedef struct processNode {
    pcb_t info;
    struct processNode *next;
} node_t;

typedef struct processList {
    node_t *first;
    node_t *last;
    uint64_t size;
    uint64_t nReady;
} pList_t;

static uint64_t pidCount = FIRST_PID;
static uint8_t firstProcess = 1;
static pList_t processList;
static node_t *currentProcess;
static node_t *noProcess;
static uint64_t lastGivenPid = 1;
static processList * readyList;
static processList * blockedList;
static process * executingP;

static int firstProcess(int argc, char **argv);
static uint64_t initProcess(process *pNode, char *name, int * fd, mode processMode);
static void setStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), uint64_t pid);
static void enqProcess(processList * list, process * p);
static uint64_t getPidOf(process * p);
static uint64_t getNewPid();

static uint64_t generatePid();
static process * getNext(processList * list);
static process * getProcess(processList * list, pid_t pid);
static process * getProcessAndPrevious(processList * list, pid_t pid, process ** prev);
static void setArgs(char ** to, char ** from, int argc);
static void exitPs();
static processList * initializeProcessList();
static int getTicketsLeft(process * p);
static uint8_t getPriority(process * p);

static void copyArguments(char **dest, int argc, char **argv);
static void addProcess(node_t *node);
static node_t *removeProcess();
static void loaderFunction(int argc, char **argv, void (*f)(int, char **));
static node_t *searchNode(uint64_t pid);
static node_t *getReadyNode();
static void exitProcess();
static int existsInList(processList * list, uint64_t pid);
static void freeProcess(node_t *node);
static int changeState(uint64_t pid, State newState);
static void printProcessListInfo(processList * list);
static void printProcessInfo(process * p);
static void printPriority(uint8_t priority);
_Noreturn static void noProcessFunction(int argc, char **argv);
static void setRemainingTime(node_t *node);


void initializeScheduler() {
    processList.first = NULL;
    processList.last = NULL;
    processList.nReady = 0;
    processList.size = 0;
    currentProcess = NULL;
    char *name[] = {"noProcess"};
    createProcess((void *)&noProcessFunction, 1, argv, NULL, FOREGROUND);
    noProcess = removeProcess();
}

void initScheduler() {
    //ncPrintWithColor("INICIALIZANDO SCHEDULER\n", 0x02);

     /* Se inicializa la lista circular de READY */
    readyList = initializeProcessList();
    if(readyList == NULL)
        return;
    /* Se inicializa la lista circular de BLOCKED */
    blockedList = initializeProcessList();
    if(blockedList == NULL)
        return;
    /* Se agrega el primer proceso manualmente */
    createFirstProcess();
    executingP = readyList->first;
}

void createFirstProcess(){
    char *argv[] = {"firstProcess"};
    uint64_t fpPid = createProcess((void *)&firstProcess, 1, argv, NULL, FOREGROUND);
    kill(fpPid);
}

uint64_t createProcess(void (*pFunction)(int, char **), int argc, char **argv, int * fd, mode processMode){
    
    /* Reservo espacio para el nuevo nodo de proceso. Notemos que new incluye
     * al proceso y al stack del mismo */ 
    process * new = malloc(sizeof(process) + PROCESS_STACK_SIZE);
    if(new == NULL)
        return 0;
    /* El primer parametro de argv es el nombre del proceso. Lo guardo en
     * prName e inicializo el proceso new con dicho nombre */
    char * prName = argv[0];
    if(initProcess(new, prName, fd, processMode) == 0)
        return 0;

    /* Reservamos espacio para 'argc' argumentos */
    char **prArgs = malloc(sizeof(char *) * argc);
    if(prArgs == NULL)
        return 0;

    /* Copiamos uno por uno los argumentos recibidos como parametro a prArgs */
    setArgs(prArgs, argv, argc);
    /* Creamos un stack frame para el proceso, simulando que 'siempre existio' */
    setStackFrame(argc, prArgs, new, pFunction, new->pc.pid);

    new->pc.argc = argc;
    new->pc.argv = prArgs;

    for (int i = 0; i < 10; i++){
        new->pc.allocated[i] = NULL;
    }
    
    /* Se agrega el nuevo proceso a la lista*/
    enqProcess(readyList, new);
    return getPidOf(new);
}

void forceExitAfterExec(int argc, char *argv[], void *processFn(int, char **)) {
    processFn(argc, argv);
    exitPs();
}

uint64_t schedule(uint64_t rsp) {
    if (currentProcess == NULL) {
        if (processList.size == 0) {
            currentProcess = noProcess;
            setRemainingTime(currentProcess);
        } else {
            currentProcess = removeProcess();
            setRemainingTime(currentProcess);
            addProcess(currentProcess);
        }
    } else {
        if (!firstProcess)
            currentProcess->info.rsp = rsp;
        else
            firstProcess = 0;
        if (currentProcess->info.remainingCPUTime <= 0 && processList.nReady > 0) {
            currentProcess = getReadyNode();
            setRemainingTime(currentProcess);
            addProcess(currentProcess);
        } else if (processList.nReady <= 0) {
            currentProcess = noProcess;
            setRemainingTime(currentProcess);
        }
    }
    currentProcess->info.remainingCPUTime--;
    return currentProcess->info.rsp;
}

uint64_t getPid() {
    return currentProcess->info.pid;
}

uint64_t block(uint64_t pid) {
    if (pid <= FIRST_PID)
        return -1;
    return changeState(pid, BLOCKED);
}

uint64_t unblock(uint64_t pid) {
    if (pid <= FIRST_PID)
        return -1;
    return changeState(pid, READY);
}

uint64_t kill(uint64_t pid) {
    if (pid <= FIRST_PID)
        return -1;
    return changeState(pid, KILLED);
}

int exists(uint64_t pid){
    if(pid > lastGivenPid || pid < 1)
        return FALSE;
    
    int aux = existsInList(readyList, pid);
    if(aux == FALSE)
        aux = existsInList(blockedList, pid);

    return aux;
}

void wait(uint64_t pid){
    while (exists(pid)){
        yield();
    }
}

void yield() {
    currentProcess->info.remainingCPUTime = 0;
    forceTimerTick();
}


uint64_t toggleBlocked(uint64_t pid) {
    int aux = block(pid);
    if(aux == 0)
        aux = unblock(pid);
    return aux;
}

uint64_t nice(uint64_t pid, uint64_t newPriority) {
    if (newPriority < MIN_PRIORITY || newPriority > MAX_PRIORITY)
        return -1;
    node_t *node;
    if (currentProcess->info.pid == pid)
        node = currentProcess;
    else {
        node = searchNode(pid);
        if (node == NULL)
            return -1;
    }
    node->info.priority = newPriority;
    return 0;
}

int getFdIn(){
    return executingP->pc.fdIn;
}

int getFdOut(){
    return executingP->pc.fdOut;
}

int getProcessMode(){
    return executingP->pc.mode;
}

process * getExecutingP(){
    return executingP;
}

void printAllProcessesInfo(){
    if(readyList->size == 0 && blockedList->size == 0){
        print("No processes to show\n");
        return;
    }
    
    printProcessListInfo(readyList);
    printProcessListInfo(blockedList);
}

//---------------------------------------------------------
//metodos estaticos
//---------------------------------------------------------

static int firstProcess(int argc, char **argv) {
    while (1)
        _hlt();

    return 0;
}

static uint64_t initProcess(process *pNode, char *name, int * fd, mode processMode) {
    processContext *pc = &(pNode->pc);
    
    /* Genero un nuevo PID para el proceso */
    pc->pid = generatePid();
    /* Copio el nombre recibido por parametro al campo name de pc */
    memcpy(pc->name, name, strlen(name)+1);

    pc->argc = 0;
    pc->argv = NULL;
    
    /* rbp apunta a la base del stack */
    pc->rbp = (uint64_t)pNode + PROCESS_STACK_SIZE + sizeof(process);
    /* Stack pointer apunta a la primera direccion libre del stack: despues del
     * stackFrame */
    pc->rsp = (uint64_t)(pc->rbp - sizeof(stackFrame));

    /* Todos los procesos comienzan en READY */
    pc->state = READY;
    /* Todos los procesos comienzan con la DEFAULT_PRIORITY */
    pc->priority = DEFAULT_PRIORITY;
    pc->ticketsLeft = initialTickets(pc->priority);
    pc->mode = processMode;

    if(fd == NULL){
        pc->fdIn = STDIN;
        pc->fdOut = STDOUT;
    } else {
        pc->fdIn = fd[0];
        pc->fdOut = fd[1];
    }

    if(processMode == BACKGROUND){
        pc->fdOut = -1;
    }
    return pc->pid;
}

static void setStackFrame(int argc, char **argv, process *pNode, void (*processFn)(int, char **), pid_t pid) {
    stackFrame *stack = (stackFrame *)(pNode->pc.rsp);

    stack->r15 = 0x1;
    stack->r14 = 0x2;
    stack->r13 = 0x3;
    stack->r12 = 0x4;
    stack->r11 = 0x5;
    stack->r10 = 0x6;
    stack->r9 = 0x7;
    stack->r8 = 0x8;

    stack->rsi = (uint64_t)argv;
    stack->rdi = argc;
    stack->rbp = 0;
    stack->rdx = (uint64_t)processFn;
    stack->rcx = pid;
    stack->rip = (uint64_t)forceExitAfterExec;
    stack->cs = 0x8;
    stack->rflags = 0x202;
    stack->rsp = (uint64_t)(pNode->pc.rsp);
    stack->ss = 0x0;
}

static void enqProcess(processList * list, process * p) {
    /* Si la lista no existe o el proceso es nulo, no puedo encolarlo */
    if(p == NULL || list == NULL)
        return;
    
    /* Si no habia procesos en la lista, este es el primero */
    if(list->size == 0) {
        list->first = p;
        list->iterator = p;
    /* Si no, es el nuevo ultimo */
    } else {
        list->last->next = p;
    }
    
    /* Actualizo el puntero al ultimo (que ahora es este proceso)*/
    list->last = p;
    /* Como es lista circular, el siguiente del ultimo es el primero */
    p->next = list->first;

    list->size++;
}

static uint64_t getNewPid() {
    return ++pidCount;
}

static void copyArguments(char **dest, int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        size_t strsize = strlen(argv[i]);
        dest[i] = memAlloc(strsize + 1);
        if (argv[i] == NULL)
            return;
        memcpy(dest[i], argv[i], strsize + 1);
    }
}

static void addProcess(node_t *node) {
    if (node == NULL)
        return;
    if (processList.first == NULL)
        processList.first = node;
    else
        processList.last->next = node;
    processList.last = node;
    node->next = NULL;
    processList.size++;
    if (node->info.state == READY)
        processList.nReady++;
}

static node_t *removeProcess() {
    if (processList.size == 0)
        return NULL;

    node_t *ans = processList.first;
    if (processList.last == ans)
        processList.last = NULL;

    processList.first = processList.first->next;
    if (ans->info.state == READY)
        processList.nReady--;
    processList.size--;
    return ans;
}

static void loaderFunction(int argc, char **argv, void (*f)(int, char **)) {
    f(argc, argv);
    exitProcess();
}

static node_t *searchNode(uint64_t pid) {
    node_t *aux = processList.first;
    while (aux != NULL) {
        if (aux->info.pid == pid)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

static node_t *getReadyNode() {
    node_t *ret = removeProcess();
    while (ret->info.state != READY) {
        if (ret->info.state == KILLED)
            freeProcess(ret);
        else
            addProcess(ret);
        ret = removeProcess();
    }
    return ret;
}

static void exitProcess() {
    kill(currentProcess->info.pid);
}

static int existsInList(processList * list, uint64_t pid){
    process * current = list->first;

    for (int i = 0; i < list->size; i++){
        if(current->pc.pid == pid)
            return TRUE;
        current = current->next;
    }

    return FALSE;    
}

static void freeProcess(node_t *node) {
    for (int i = 0; i <= node->info.argc; ++i) {
        memFree(node->info.argv[i]);
    }
    memFree(node->info.argv);
    memFree(node->info.stackMem);
    memFree(node);
}

static int changeState(uint64_t pid, State newState) {
    node_t *aux = searchNode(pid);
    if (aux == NULL || aux->info.state == KILLED)
        return -1;

    if (aux->info.state == newState)
        return 1;

    if (newState == READY && aux->info.state != READY)
        processList.nReady++;
    else if (aux->info.state == READY && newState != READY)
        processList.nReady--;

    if (newState == KILLED)
        freeProcess(aux);

    return 0;
}


static void printProcessListInfo(processList * list) {
    process * toPrint;
    if(list->size > 0){
        toPrint = list->first;
        for (int i = 0; i < list->size; i++){
            printProcessInfo(toPrint);
            toPrint = toPrint->next;
        }
    }
}

static void printProcessInfo(process * p){
    printDec(getPidOf(p));
    print(TAB);

    print(p->pc.name);
    int length = strlen(p->pc.name);
    if(length < PROCESS_NAME_PRINT_SIZE){
        for(int i=0; i < PROCESS_NAME_PRINT_SIZE - length; i++)
            putChar(' ');
    }
    print(TAB);

    printHex(p->pc.rsp);
    print(TAB);

    printHex(p->pc.rbp);
    print(TAB);

    switch(p->pc.state) {
        case READY: 
            print("READY");
            break;
        case BLOCKED:
            print("BLOCKED");
            break;
        default:
            print("?????");
    }
    print(TAB);
    print(TAB);
    printPriority(getPriority(p));

    putChar('\n');
}

static void printPriority(uint8_t priority){
    char c1 = (priority/10) + '0';
    char c2 = (priority%10) + '0';

    putChar(c1);
    putChar(c2);
}

_Noreturn static void noProcessFunction(int argc, char **argv) {
    while (1)
        _hlt();
}

static void setRemainingTime(node_t *node) {
    node->info.remainingCPUTime = node->info.priority * QUANTUM;
}

static uint64_t generatePid() {
    return lastGivenPid++;
}

static process * getNext(processList * list) {
    if(list == NULL || list->size == 0)
        return NULL;
    
    process * next = list->iterator;
    list->iterator = list->iterator->next;

    return next;
}

static process * getProcess(processList * list, uint64_t pid){
    process * toDiscard = NULL;
    return getProcessAndPrevious(list, pid, &toDiscard);
}

/* Obtiene un proceso especifico de la lista por PID, pero no lo elimina de la misma.
 * Adicionalmente, en el puntero *previous devuelve el nodo anterior al retornado */
static process * getProcessAndPrevious(processList * list, uint64_t pid, process ** prev){
    /* Si la lista no existe, esta vacia o el PID es invalido, retorno */
    if(list == NULL || list->size == 0 || pid <= 0)
        return NULL;

    /* Tomo el primero. En este punto, no puede ser NULL */
    process * toRet = list->first;
    *prev = toRet;
    /* Recorro la lista buscando aquel proceso que coincida en PID */
    int i;
    for (i = 0; i < list->size; i++){
        if(toRet->pc.pid == pid)
            break;
        *prev = toRet;
        toRet = toRet->next;
    }

    /* Si se recorrio toda la lista, significa que no se encontro */
    if(i == list->size){
        *prev = NULL;
        return NULL;
    }

    return toRet;
}

static void setArgs(char ** to, char ** from, int argc){
    int argLen;

    for (int i = 0; i < argc; i++){
        argLen = strlen(from[i]);

        /* Reservo espacio para el argumento i */
        to[i] = malloc(argLen + 1);
        if(to[i] == NULL)
            return;

        /* Copio el argumento i al nuevo arreglo */
        memcpy(to[i], from[i], argLen+1);
    }  
}

static void exitPs(){
    kill(executingP->pc.pid);
}

static processList * initializeProcessList() {
    processList * list = malloc(sizeof(processList));
    if(list == NULL)
        return NULL;
    list->first = NULL;
    list->last = NULL;
    list->iterator = NULL;
    list->size = 0;

    return list;
}

static int initialTickets(int priority) {
    return (MIN_PRIORITY + 1 - priority) * TQ; 
}

static int getTicketsLeft(process * p) {
    if(p == NULL)
        return 0;
    return p->pc.ticketsLeft;
}

static uint8_t getPriority(process * p) {
    if(p == NULL)
        return 0;
    return p->pc.priority;
}


