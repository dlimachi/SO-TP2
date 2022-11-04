# Manual de usuario #

## Compilación
Este kernel debe correrse a través del emulador qemu y se debe compilar desde docker a través del contenedor 'agodio/itba-so:1.0', para ello una vez abierta la terminal de dicha aplicación se deben seguir los siguientes pasos:

1. La primera vez que uno accede debe ejecutar `cd Toolchain` y luego `make all`.

2. Luego resta compilar todo el proyecto desde el directorio principal, para ello si todavia se encuentra en `Toolchain` ejecute `cd ..`.

3. *(OPCIONAL)* Por defecto este sistema utiliza un memory manager personalizado, si usted desea seguir con este pase al paso 4. Si usted quiere compilar y utlizar el buddy memory manager, deberá ejecutar `cd Kernel` y allí dentro `make MM=BUDDY all` para luego volver al directorio principal con `cd ..`.

4. Por último, deberá ejecutar `make all` desde el directorio principal para compilar y linkeditar todo el proyecto. 

5. Solo resta ejecutar el archivo `run.sh`, que contiene el script con el comando correspondiente para correr al sistema a través de qemu.


## Terminal

Una vez ejecutado el run.sh, se abrirá una ventana con una terminal donde usted dispondrá de los siguientes comandos:

- ***help***: Despliega en pantalla todos los comandos disponibles para el usuario con su respectiva descripción.

- ***inforeg***: Muestra en pantalla el valor de los registros, si es que se guardó previamente. Para guardar el estado de los registros en un momento específico, simplemente debe presionar la tecla `Shift`.

- ***printmem***: Realiza un dump de 32 bytes de una zona de memoria que recibe por parámetro. 

- ***time***: Despliega la hora y fecha actual.

- ***clear***: Limpia la pantalla, eliminando los comandos anteriores dejando a la terminal en su estado inicial, lista para recibir un nuevo comando.

- ***divzero***: Verifica el correcto funcionamiento de la excepción de división por cero.

- ***invalidopcode***: Verifica el correcto funcionamiento de la excepción de código de operación invalido.

- ***phylo***: Resuelve el problema de los filósofos.

- ***mem***: Imprime el estado de la memoria.

- ***ps***: Imprime la lista de todos los procesos en ejecución con sus propiedades.

- ***sleep***: Retrasa el funcionamiento de la terminal una cantidad especifica de segundos.

- ***loop***: Imprime el ID del proceso actual con un saludo cada una cantidad de segundos.

- ***block***: Cambia el estado de un proceso entre BLOCKED y READY dado su ID.

- ***kill***: Mata un proceso dado su ID.

- ***nice***: Cambia la prioridad de un proceso dado su ID y la nueva prioridad.

- ***block***: Cambia el estado de un proceso entre bloqueado y listo dado su ID.

- ***kill***:  Mata un proceso dado su ID.

- ***sem***: Imprime la lista de todos los semáforos con sus propiedades.

- ***testmm***: Test para el memory manager.

- ***testprocesses***: Test para la creación de procesos.

- ***testpriority***: Test para la prioridad de los procesos.

- ***testsynchro***: Test para la sincronización de los procesos con semáforos.

- ***testnosynchro***: Test para la sincronización de los procesos sin semáforos.

- ***cat***: Imprime el stdin tal como lo recibe.

- ***wc***: Cuenta la cantidad de líneas del input.

- ***filter***: Filtra las vocales del input.

- ***pipe***: : Imprime la lista de todos los pipes con sus propiedades.


## Syscalls

[0] char ***sys_getChar***();
Devuelve un caracter ingresado por el usuario o 0 en su defecto.

[1] extern void ***sys_putCharWC***(char c, unsigned char colorCode);
Ubica el caracter 'c' en la posicion actual de la pantalla con el color indicado en 'colorCode'.

[2] int ***sys_getTime***(int descriptor);
Devuelve el campo de la fecha/hora actual indicado en 'descriptor'.

[3] void ***sys_clear***();
Elimina todo el contenido de la pantalla y posiciona el prompt al comienzo de la misma.

[4] void ***sys_saveRegs***();
Crea un backup de los registros para luego imprimirlos cuando se llame `inforeg`.

[5] void ***sys_PrintMem***(char * address);
Imprime en pantalla el contenido de 32 bytes de memoria a partir de la direccion 'address'.

[6] void ***sys_printCharInPos***(char c, int row, int col, unsigned char colorCode);
Imprime el caracter 'c' en la fila 'row' y columna 'col' de la pantalla, teniendo en cuenta que la misma tiene un tamaño de 25x80. Ademas, lo hace en el color indicado por 'colorCode'.

[7] unsigned long ***sys_getDeciseconds***();
Retorna los decisegundos desde que se inicio el sistema.

[8] void ***sys_getRegistersInfo***();
Imprime en pantalla cada registro del procesador con su contenido.

[9] void ***sys_ps***();
Imprime una lista con todos los procesos en ejecución.

[10] unsigned long long ***sys_getPid***();
Retorna el Process ID del proceso actual.

[11] void ***sys_togglePsState***(unsigned long long pid);
Cambia el estado de un proceso entre bloqueado y listo dado su ID.

[12] void ***sys_killPs***(unsigned long long pid);
Mata un proceso dado su ID.

[13] int ***sys_getSeconds***();
Retorna los segundos desde que se inicio el sistema.

[14] void ***sys_mem***();
Imprime el estado actual de la memoria.

[15] unsigned long long ***sys_createProcess***(void (*pFunction)(int, char **), int argc, char **argv, int * fd, mode processMode);
Crea un nuevo proceso y devuelve su PID.

[16] void ***sys_sem***();
Imprime el estado actual de la memoria.

[17] void ***sys_nice***(unsigned long long pid, char newPriority);
Cambia la prioridad de un proceso dado su ID y la nueva prioridad.

[18] void ***sys_yield***();
El proceso que la llama renuncia a la CPU forzando una interrupcion del timer tick.
.
[19] void * ***sys_malloc***(uint32_t size);
Reserva un tamaño de memoria pasado por parámetro.

[20] void ***sys_free***(void *pr);
Libera la direccion de memoria apuntada por el puntero pasado por parámetro.

[21] unsigned long ***sys_sem_open***(char *sem_name, uint64_t initialValue);
Crea un nuevo semáforo con su respectivo nombre y valor inicial pasados por parámetro.

[22] unsigned long ***sys_sem_close***(char *sem_name);
Cierra un semáforo dado su nombre.

[23] unsigned long ***sys_sem_wait***(char *sem_name);
Decrementa en 1 el valor del semáforo.

[24] unsigned long ***sys_sem_post***(char *sem_name);
Incrementa en 1 el valor del semáforo.

[25] unsigned long ***sys_block***(uint64_t pid);
Bloquea un proceso dado su PID.

[26] unsigned long ***sys_unblock***(uint64_t pid);
Desbloquea un proceso dado su PID.

[27] void ***sys_pipe***();
Imprime en pantalla la información de todos los pipes.

[28] void ***sys_wait***(uint64_t pid);
Espera a la finalizacióm del proceso con el ID 'pid'.

[29] int ***sys_exists***(uint64_t pid);
Devuelve 1 si existe el proceso con el ID 'pid' y 0 si no.

[30] int *__*sys_pipeOpen*__(char *name);
Dado un nombre, si no existe ese valor para un nombre de pipe, crea un pipe con el nombre proveído. Siempre abre el pipe luego, sea nuevo o ya existente.

[31] uint64_t ***sys_pipeClose***(char *name);
Cierra un pipe permanentemente dado su ID.

[32] int ***sys_get_fdin***();
Devuelve el file descriptor para el input del proceso actualmente en ejecución.

[33] int ***sys_get_fdout***();
Devuelve el file descriptor para el output del proceso actualmente en ejecución.

[34] unsigned char ***sys_read***(int fd);
Si fd es 0, devuelve un caracter ingresado por el usuario o 0 en su defecto, sino lee un caracter del pipe del file descriptor dado.

[35] uint32_t ***sys_write***(int fd, char *str, uint8_t colorCode);
Si fd es 1, escribe 'str' en la posicion actual de la pantalla con el color indicado en 'colorCode',  sino escribe 'str' en el pipe del file descriptor dado.

[36] void ***sys_putChar***(char c);
Escribe un caracter en la terminal o en un pipe según corresponda.

[37] void ***sys_print***(char *rsi);
Escribe una cadena de caracteres en la terminal o en un pipe según corresponda.

[38] void ***sys_printWithColor***(char *str, uint8_t colorCode);
Escribe una cadena de caracteres en la terminal con color definido por 'colorCode' o en un pipe según corresponda.

[39] uint32_t ***sys_writeChar***(int fd, char c, uint8_t colorCode);
Si fd es 1, ubica el caracter 'c' en la posicion actual de la pantalla con el color indicado en 'colorCode', sino escribe un caracter en el pipe del file descriptor dado.