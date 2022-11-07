// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>

static void philosopher(int argc, char ** argv);
static void eat(int id);
static void leave(int id);
static void printStatus();
static void addPhil();
static void removePhil();
static void createPhilProcess();

int philoCount = 0;
static char forks[MAX_PHYL][3];
static char status[MAX_PHYL];
static unsigned long long philoPids[MAX_PHYL] = {0};

void phylo_main() {

    /* Inicializamos los 'forks' (tenedores). El nombre del semaforo de cada
     * cubierto sera igual a i */
    for (int i = 0; i < MAX_PHYL; i++){
        char semName[3];
        itoa(i, semName);

        forks[i][0] = semName[0];
        forks[i][1] = semName[1];
        forks[i][2] = semName[2];

        /* Abrimos los semaforos */
        if(sys_sem_open(forks[i], 1) == 0){
            print("Error creando tenedores\n");

            for (int j = 0; j < i; j++)
                sys_sem_close(forks[j]);  
            return;
        }
    }

    /* Creamos el semaforo del comedor (room). El mismo admite hasta philCount-1
     * filosofos, evitando el deadlock */
    if(sys_sem_open(ROOM_SEM_NAME, INITIAL_PHYL-1) == 0){
        for (int j = 0; j < MAX_PHYL; j++)
                sys_sem_close(forks[j]);  
        return;
    };
    
    /* Inicialmente, ningun filosofo esta comiendo */
    print("\n");
    for (int i = 0; i < MAX_PHYL; i++){
        status[i] = WAITING_CHAR;
    }
    printStatus();

    /* Creamos los procesos filosofos iniciales */
    for (int i = 0; i < INITIAL_PHYL; i++){
        createPhilProcess();
    }

    /* Mientras algun filosofo siga comiendo, se podran agregar o quitar filosofos
     * en tiempo de ejecucion */
    println("Escribir 'a' para agregar, escribir 'r' para borrar, escribir 'x' para salir.");
    for (int i = 0; i < MAX_PHYL; i++){
        char c;
        while (sys_exists(philoPids[i])){
            c = getCharOrNull();
            if(c == ADD_PHILO_KEY){
                //sys_sem_wait(CAN_CHANGE_SEM);
                addPhil();
                //sys_sem_post(CAN_CHANGE_SEM);
            }
            else if (c == REMOVE_PHILO_KEY){
                if (philoCount > MIN_PHYLOS) {
                    //sys_sem_wait(CAN_CHANGE_SEM);
                    removePhil();
                    //sys_sem_post(CAN_CHANGE_SEM);
                }
            }
        }
    }
    
    /* Antes de terminar, cerramos todos los semaforos */
    for (int i = 0; i < MAX_PHYL; i++)
        sys_sem_close(forks[i]);
    
    sys_sem_close(ROOM_SEM_NAME);
    //sys_sem_close(CAN_CHANGE_SEM);

    philoCount = 0;
}

void philosopher(int argc, char ** argv){
    
    int id = atoi(argv[1]);
    int lFork = id;
    int rFork = (id+1) % philoCount;

    while (1) {
        sys_sem_wait(ROOM_SEM_NAME);
        sys_sem_wait(forks[lFork]);
        sys_sem_wait(forks[rFork]);

        if (id % 2 == 0) {
            lFork = (id+1) % philoCount;
            rFork = id;
        } else {
            lFork = id;
            rFork = (id-1) % philoCount;
        }

        for (uint64_t i = 0; i < 50000000; i++);

        sys_sem_wait(forks[lFork]);
        sys_sem_wait(forks[rFork]);

        status[id] = 1;
        eat(id);
        leave(id);

        for (int i = 0; i < philoCount; i++) {
            printStatus();
        }
        print("");

        for (uint64_t i = 0; i < 50000000; i++);

        status[id] = 0;

        sys_sem_post(forks[lFork]);
        sys_sem_post(forks[rFork]);
        sys_sem_post(ROOM_SEM_NAME);
    }
}

static void eat(int id){
    status[id] = EATING_CHAR;
    printStatus();
    sys_yield();
}

static void leave(int id){
    status[id] = EXITED_CHAR;
    printStatus();
}

static void printStatus(){
    for (int i = 0; i < philoCount; i++){
         /*switch (status[i]){
        case EATING_CHAR:
            putCharWithColor(status[i], CYAN_BLACK);
            break;
        case EXITED_CHAR:
            putCharWithColor(status[i], MAGENTA_BLACK);
            break;
        default:
            putCharWithColor(status[i], ORANGE_BLACK);
        }*/
        if (status[i] == EXITED_CHAR)
            return;
        if (status[i] == EATING_CHAR)
            putChar('E'); 
        else
            putChar('.');
            
        putChar(' ');
    }
    putChar('\n');
}

static void addPhil(){
    if(philoCount == MAX_PHYL)
        return;

    createPhilProcess();
    sys_sem_post(ROOM_SEM_NAME);
}

static void removePhil(){
    if(philoCount == INITIAL_PHYL)
        return;

    sys_killPs(philoPids[philoCount]);
    philoPids[philoCount] = 0;
    philoCount--;
}

static void createPhilProcess(){
    char id[] = {philoCount+'0', 0};
    char * philoArgv[] = {"philosopher", id};

    int processMode = sys_getProcessMode();

    philoPids[philoCount] = sys_createProcess(&philosopher, 2, philoArgv, NULL, processMode);

    if(philoPids[philoCount] == 0){
        printWithColor("Error creando filosofos\n", RED_BLACK);
        return;
    }

    philoCount++;
}