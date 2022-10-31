// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipe.h>

static void enqPipe(TPipe * pipe);
static TPipe * deqPipe();
static TPipe * getPipe(char * name);
static int * createPipe(char *name, int id);
static int generateNewFd();
static TPipe * getPipeWithFd(int fd, int fdType);

static char * handlerSemName = "pipesHandler";
static pipeList * pipesList;
static int lastFdGenerated = 2;

void initPipes(){
    if(semOpen(handlerSemName, 1) == FAILED){
        print("ERROR: failed to open semaphore in 'initPipes()'\n");
        return;
    }
    pipesList = malloc(sizeof(pipeList));
    pipesList->first = NULL;
    pipesList->last = NULL;
    pipesList->size = 0;
}

static void enqPipe(TPipe * pipe) {
    if(pipe == NULL || pipesList == NULL)
        return;
    
    if(pipesList->first == NULL)
        pipesList->first = pipe;
    else
        pipesList->last->next = pipe;
    
    pipesList->last = pipe;

    pipe->next = NULL;

    pipesList->size++;
}

static TPipe * deqPipe() {

    if(pipesList == NULL || pipesList->size == 0)
        return NULL;

    TPipe * deq = pipesList->first;

    if(pipesList->size == 1){
        pipesList->first = NULL;
        pipesList->last = NULL;
    } else {
        pipesList->first = deq->next;
    }

    deq->next = NULL;

    pipesList->size--;
    return deq;
}

static int * createPipe(char *name, int id){
    TPipe* new = malloc(sizeof(TPipe));

    if(new == NULL)
        return NULL;

    new->fds[0] = generateNewFd();
    new->fds[1] = generateNewFd();

    memcpy(new->name, name, strlen(name)+1);

    new->readIndex = 0;
    new->writeIndex = 0;

    char rSem[NAME_MAX_SIZE];
    int lenght = strlen(READ_SEM_NAME);
    memcpy(rSem, READ_SEM_NAME, lenght+1);
    rSem[lenght] = id + '0';
    rSem[lenght+1] = 0;
    int sr = semOpen(rSem, 0);

    char wSem[NAME_MAX_SIZE];
    lenght = strlen(WRITE_SEM_NAME);
    memcpy(wSem, WRITE_SEM_NAME, lenght+0);
    wSem[lenght] = id + '0';
    wSem[lenght+1] = 0;
    int sw = semOpen(wSem, 1);

    if(sr == FAILED || sw == FAILED){
        print("Error abriendo semaforos en createPipe\n");
        return NULL;
    }

    memcpy(new->readSemName, rSem, strlen(rSem)+1);
    memcpy(new->writeSemName, wSem, strlen(wSem)+1);

    new->numOfProcessesAttached = 0;


    enqPipe(new);
    return new->fds;
}

static TPipe * getPipe(char * name){
    TPipe * aux = pipesList->first;

    for(int i = 0; i < pipesList->size; i++){
        if(strcmp(name, aux->name) == 0){
            return aux;}
        aux = aux->next;
    }
    return NULL;
}

int * pipeOpen(char *name){
    if (semWait(handlerSemName) == FAILED){
        print("Error semWait en pipeOpen\n");
        return FAILED;
    }

    int * fds;
    TPipe * toOpen = getPipe(name);
    if (toOpen == NULL){
        fds = createPipe(name, pipesList->size+1);
        if(fds == NULL){
            print("Error creando pipe en pipeOpen\n");
        }
    } else{
        toOpen->numOfProcessesAttached++;
        fds = toOpen->fds;
    }
    
    semPost(handlerSemName);
    return fds;
}

uint64_t pipeClose(char * pipeName){
    if (semWait(handlerSemName) == FAILED){
        print("Error semWait en pipeClose\n");
        return FAILED;
    }

    TPipe * toClose = getPipe(pipeName);
    if(toClose == NULL){
        print("The pipe "); 
        print(pipeName);
        print("does not exist\n");
        return FAILED;
    }

    uint64_t rs = semClose(toClose->readSemName);
    uint64_t rw = semClose(toClose->writeSemName);

    if(rs == FAILED || rw == FAILED){
        print("Error semClose en pipeClose\n");
        return FAILED;
    }

    deqPipe(toClose);
    free(toClose);
    if (semPost(handlerSemName) == FAILED){
        print("Error semPost en pipeClose\n");
        return FAILED;
    }
    return SUCCESS;
}

uint32_t writePipe(char * pipeName, char *str){
    TPipe * toWrite = getPipe(pipeName);
    int written = 0;
    if(toWrite == NULL){
        print("The pipe "); 
        print(pipeName);
        print("does not exist\n");
        return FAILED;
    }

    while(*str != 0){
        if(writeCharInPipe(toWrite, *str++) == FAILED)
            return FAILED;
        written++;
    }

    return written;
}

uint64_t writeCharInPipe(TPipe * pipe, char c){
    if(semWait(pipe->writeSemName) == FAILED){
        print("Error semWait en writeCharInPipe\n");
        return FAILED;
    }

    pipe->buffer[pipe->writeIndex % BUFFER_SIZE] = c;
    pipe->writeIndex++;

    if(semPost(pipe->writeSemName) == FAILED){
        print("Error semPost en writeCharInPipe\n");
        return FAILED;
    }
    semPost(pipe->readSemName);

    return SUCCESS;
}

uint64_t writeCharInPipeWithFd(int fd, char c){
    TPipe * toWrite = getPipeWithFd(fd, FDIN);
    if(toWrite == NULL)
        return FAILED;

    if(semWait(toWrite->writeSemName) == FAILED){
        print("Error semWait en writeCharInPipe\n");
        return FAILED;
    }

    toWrite->buffer[toWrite->writeIndex % BUFFER_SIZE] = c;
    toWrite->writeIndex++;

    if(semPost(toWrite->writeSemName) == FAILED){
        print("Error semPost en writeCharInPipe\n");
        return FAILED;
    }
    semPost(toWrite->readSemName);

    return SUCCESS;
}



uint32_t writeInPipeWithFd(int fd, char *str){
    TPipe * toWrite = getPipeWithFd(fd, FDIN);
    int written = 0;
    if(toWrite == NULL){
        print("The pipe "); 
        print(toWrite->name);
        print("does not exist\n");
        return FAILED;
    }

    while(*str != 0){
        if(writeCharInPipe(toWrite, *str++) == FAILED)
            return FAILED;
        written++;
    }
    toWrite->buffer[toWrite->writeIndex % BUFFER_SIZE] = 0;

    return written;
}

char readPipe(char * pipeName){
    TPipe * toRead = getPipe(pipeName);
    if(toRead == NULL){
        print("The pipe "); 
        print(pipeName);
        print("does not exist\n");
        return FAILED;
    }
    if(semWait(toRead->readSemName) == FAILED){
        print("Error semWait en readPipe\n");
        return FAILED;
    }

    char toRet = toRead->buffer[toRead->readIndex % BUFFER_SIZE];
    toRead->readIndex++;

    if(semPost(toRead->readSemName) == FAILED){
        print("Error semPost en readPipe\n");
        return FAILED;
    }

    return toRet;
}

char readPipeWithFd(int fd){
    TPipe * toRead = getPipeWithFd(fd, FDOUT);
    if(toRead == NULL)
        return FAILED;

    if(semWait(toRead->readSemName) == FAILED){
        print("Error semWait en readPipe\n");
        return FAILED;
    }

    char toRet = toRead->buffer[toRead->readIndex % BUFFER_SIZE];
    toRead->readIndex++;

    if(semPost(toRead->readSemName) == FAILED){
        print("Error semPost en readPipe\n");
        return FAILED;
    }

    return toRet;
}



static TPipe * getPipeWithFd(int fd, int fdType){
    if(fd < 2 || fd > lastFdGenerated)
        return NULL;
    if(fdType != FDIN && fdType != FDOUT)
        return NULL;
    
    TPipe * toRet = pipesList->first;
    for (int i = 0; i < pipesList->size; i++){
        if(toRet->fds[fdType] == fd)
            return toRet;
    }
    return NULL;
}



void printListOfPipes(){
    TPipe * toPrint = pipesList->first;

    if(toPrint == NULL){
        print("No pipes to show\n");
        return;
    }
    printWithColor("NAME        FDIN      FDOUT\n", ORANGE_BLACK);
    for(int i = 0; i < pipesList->size; i++){
        print(toPrint->name);
        print("        ");
        printDec(toPrint->fds[0]);
        print("          ");
        printDec(toPrint->fds[1]);
        print("          ");
        toPrint = toPrint->next;
        putChar('\n');
    }
}

static int generateNewFd(){
    return lastFdGenerated++;
}