// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphores.h>

static int createSemaphore(char *name, int initialValue);
static void enqSem(TSem * sem);
static TSem * delSem(char * semName);
static void enqPr(TSem * sem, pid_t pid);
static pid_t deqPr(TSem * sem);
static TSem * getSemAndPrevious(char * semName, TSem ** prev);
static TSem * getSem(char * name);
static TSem * getSemAndPrevious(char * semName, TSem ** prev);
static void freeSem(TSem * sem);

static uint64_t semLock = 0;
static semList * semaphoresList;

void initSemaphores(){
    semaphoresList = malloc(sizeof(semList));
    semaphoresList->first = NULL;
    semaphoresList->last = NULL;
    semaphoresList->size = 0;
}

static int createSemaphore(char *name, int initialValue){
    TSem * new = malloc(sizeof(TSem));
    if(new == NULL)
        return FAILED;

    memcpy(new->name, name, strlen(name)+1);

    new->value = initialValue;
    new->lock = 0;
    new->firstProcess = NULL;
    new->lastProcess = NULL;
    new->waitingProcesses = 0;
    new->openedBy = 1;

    enqSem(new);
    return SUCCESS;
}

static void enqSem(TSem * sem) {
    if(sem == NULL || semaphoresList == NULL)
        return;
    
    if(semaphoresList->first == NULL)
        semaphoresList->first = sem;
    else
        semaphoresList->last->next = sem;
    
    semaphoresList->last = sem;
    sem->next = NULL;

    semaphoresList->size++;
}

static TSem * delSem(char * semName) {

    if(semaphoresList == NULL || semaphoresList->size == 0)
        return NULL;

    TSem * prev = NULL;
    TSem * toDel = getSemAndPrevious(semName, &prev);
    if(toDel == NULL)
        return NULL;
    
    if(semaphoresList->size == 1){
        semaphoresList->first = NULL;
        semaphoresList->last = NULL;
    } else {
        if(toDel == semaphoresList->first)
            semaphoresList->first = semaphoresList->first->next;
        else if(toDel == semaphoresList->last)
            semaphoresList->last = prev;

        prev->next = toDel->next;
    }

    toDel->next = NULL;
    semaphoresList->size--;

    return toDel;
}

static TSem * getSemAndPrevious(char * semName, TSem ** prev){
    if(semaphoresList == NULL || semaphoresList->size == 0 || semName == NULL)
        return NULL;

    TSem * toRet = semaphoresList->first;
    *prev = toRet;

    int i;
    for (i = 0; i < semaphoresList->size; i++){
        if(strcmp(toRet->name, semName) == 0)
            break;
        *prev = toRet;
        toRet = toRet->next;
    }

    if(i == semaphoresList->size){
        *prev = NULL;
        return NULL;
    }

    return toRet;
}


static void enqPr(TSem * sem, pid_t pid){
    
    pNode * process = malloc(sizeof(pNode));

    if(process == NULL)
        return;

    process->pid = pid;

    if(sem->firstProcess == NULL)
        sem->firstProcess = process;
    else
        sem->lastProcess->next = process;

    sem->lastProcess = process;
    process->next = NULL;

    sem->waitingProcesses++;
}

static pid_t deqPr(TSem * sem){
    if(sem->waitingProcesses == 0)
        return FAILED;

    pNode * process = sem->firstProcess;
    int pid = process->pid;

    if(sem->waitingProcesses == 1){
        sem->firstProcess = NULL;
        sem->lastProcess = NULL;
    } else {
        sem->firstProcess = process->next;
    }

    process->next = NULL;
    free(process);
    sem->waitingProcesses--;
    return pid;
}

static TSem * getSem(char * semName){
    TSem * toDiscard = NULL;
    return getSemAndPrevious(semName, &toDiscard);
}

uint64_t semOpen(char *name, int initialValue){
    _xchgLock(&semLock);

    TSem * toOpen = getSem(name);
    if(toOpen == NULL){
        if(createSemaphore(name, initialValue) == FAILED){
            _unlock(&semLock);
            printWithColor("Error al crear semaforo\n", RED_BLACK);
            return FAILED;
        }
    } else {
        toOpen->openedBy++;
    }

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semClose(char * semName){
    _xchgLock(&semLock);


    TSem * toClose = getSem(semName);
    if(toClose == NULL){
        print("The Semaphore "); 
        print(semName);
        print("does not exist\n");
        return FAILED;
    }


    toClose->openedBy--;
    if(toClose->openedBy == 0){
        delSem(semName);
        freeSem(toClose);
    }

    _unlock(&semLock);
    return SUCCESS;
}

static void freeSem(TSem * sem){
    if(sem == NULL)
        return;

    pNode * toFree = sem->firstProcess;
    for (int i = 0; i < sem->waitingProcesses; i++){
        if(toFree == NULL)
            break;
        free(toFree);
        toFree = toFree->next;        
    }
    free(sem);
}

uint64_t semWait(char * semName){
    _xchgLock(&semLock);

    TSem * toWait = getSem(semName);

    if(toWait == NULL){
        print("WAIT: No existe el semaforo ");
        print(semName);
        putChar('\n');
        _unlock(&semLock);
        return FAILED;
    }

    _xchgLock(&toWait->lock);
    _unlock(&semLock);

    pid_t pid = getPid();

    while(toWait->value == 0){ // tengo que bloquear el proceso  
        enqPr(toWait, pid);    // agrego el proceso en la cola del semaforo
        _unlock(&toWait->lock); // desbloqueo el lock del semaforo
        if(block(pid) == -1) {  // bloqueo el proceso
            return FAILED;}
        _xchgLock(&toWait->lock); // vuelvo a bloquear el lock semaforo
    }

    toWait->value--;     // el value era mayor a 0 -> solo lo decremento
    _unlock(&toWait->lock); 
    
    return SUCCESS;
}

uint64_t semPost(char * semName){
    _xchgLock(&semLock);

    TSem * toPost = getSem(semName);

    if(toPost == NULL){
        print("POST: No existe el semaforo ");
        print(semName);
        putChar('\n');
        _unlock(&semLock);
        return FAILED;
    }

    _xchgLock(&toPost->lock);
    _unlock(&semLock);

    toPost->value++;

    int pid = deqPr(toPost);
    if(pid == FAILED){
        _unlock(&toPost->lock);
        return SUCCESS;
    }

    _unlock(&toPost->lock);

    if(unblock(pid) == 0)
            return FAILED;
    return SUCCESS;
}

void printListofSemaphores(){
    TSem * toPrint = semaphoresList->first;

    if(toPrint == NULL){
        print("No semaphores to show\n");
        return;
    }

    printWithColor("NAME              VALUE     BLOCKED PROCESSES\n", ORANGE_BLACK);

    for(int i = 0; i < semaphoresList->size; i++){
        print(toPrint->name);
        print("        ");
        printDec(toPrint->value);
        print("            ");

        pNode * process = toPrint->firstProcess;
        for(int j = 0; j < toPrint->waitingProcesses; j++){
            if(process == NULL)
                printWithColor("Error: Proceso no encontrado", RED_BLACK);
            else
                printDec(process->pid);
            print("   ");
            process = process->next;
        }

        toPrint = toPrint->next;
        putChar('\n');
    }
}