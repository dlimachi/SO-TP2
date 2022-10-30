// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef BUDDY
#include <memManager.h>
// #include <memManagerCustom.h>


#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000

/* A cuantos bytes quiero alinear */
#define BYTE_ALIGNMENT 8 

/* Mascara para alinear */
#define BYTE_ALIGNMENT_MASK 0x07

/* Tamaño del puntero (arquitectura) */
#define POINTER_SIZE_TYPE uint64_t

/* Se pueden perder algunos bytes al alinear la primera posicion del heap. En el
 * peor caso, se pierden BYTE_ALIGNMENT bytes */
#define ADJUSTED_HEAP_SIZE (HEAP_MAX_SIZE - BYTE_ALIGNMENT)

/* Se usa para evitar un 'magic number'(8) suelto, sin significado */
#define BITS_PER_BYTE ((size_t) 8)

/* Maximo valor que se puede representar con size_t */
#define MAX_SIZE_T (~((size_t) 0))

/* Chequea que la suma entre a y b sea menor que MAX_SIZE_T */
#define MULTIPLY_WILL_OVERFLOW(a, b)    (((a) > 0) && ((b) > (MAX_SIZE_T / (a))))

/* Chequea que el producto entre a y b sea menor que MAX_SIZE_T */
#define ADD_WILL_OVERFLOW(a, b)         ((a) > (MAX_SIZE_T - (b)))


/* Se usa el bit mas significativo del blockSize (en un memoryBlock_t) para marcar
 * si un bloque esta reservado o libre. Si dicho bit esta en 1, entonces el bloque
 * esta usado. En caso contrario, esta libre */

/* Mascara para el bit mas significativo */
#define BLOCK_ALLOCATED_BITMASK    (((size_t) 1) << ((sizeof(size_t) * BITS_PER_BYTE) - 1))
/* Un tamaño de bloque es valido si no utiliza su bit mas significativo */
#define BLOCK_SIZE_IS_VALID(blockSize)    (((blockSize) & BLOCK_ALLOCATED_BITMASK) == 0)
/* Usa la mascara para revisar el bit mas significativo de blockSize y saber si
 * esta usado */
#define BLOCK_IS_ALLOCATED(memoryBlock)        (((memoryBlock->blockSize) & BLOCK_ALLOCATED_BITMASK) != 0)
/* Setea en 1 el bit mas significativo de blockSize (lo marca como usado) */
#define ALLOCATE_BLOCK(memoryBlock)            ((memoryBlock->blockSize) |= BLOCK_ALLOCATED_BITMASK)
/* Setea 0 el bit mas significativo de blockSize (lo marca como libre) */
#define FREE_BLOCK(memoryBlock)                ((memoryBlock->blockSize) &= ~BLOCK_ALLOCATED_BITMASK)

/*-----------------------------------------------------------*/

/* Direccion del heap */
static uint8_t * heap = (uint8_t *) HEAP_START;

/* Estructura que define una linked list de bloques libres, ordenada de menor a mayor
 * tamaño. Cada nodo almacena su tamanio de memoria para dar y el puntero al
 * siguiente nodo */
typedef struct memoryBlock {
    struct memoryBlock * nextBlock; 
    size_t blockSize;                     
} memoryBlock_t;

/* Tamaño alineado de la estructura mencionada arriba */
static const uint16_t MemoryBlockStructSize = ((sizeof(memoryBlock_t) + (BYTE_ALIGNMENT - 1)) & ~((size_t) BYTE_ALIGNMENT_MASK));
/* El tamaño minimo de un bloque es equivalente a dos estructuras memoryBlock_t*/
#define MINIMUM_BLOCK_SIZE    ((size_t) (MemoryBlockStructSize * 2))

/* Estos bloques se utilizan como 'marcadores', representando el comienzo
*  y fin de la lista de bloques respectivamente */
static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

/* Cantidad total de bytes disponibles para entregar (no tiene en cuenta 
 * la fragmentacion) */
static size_t freeBytesRemaining = ADJUSTED_HEAP_SIZE;

/* Variable que indica si el heap ya fue inicializado. Solo se cambia en el
 * primer pedido de memoria y queda en TRUE durante el resto de la ejecucion */
static uint64_t wasHeapInitialized = FALSE;

/* Inicializa las estructuras del heap en su primer uso */
static void initializeHeap() {
    memoryBlock_t * firstFreeBlock;
    uint8_t * alignedHeap;

    /* Se asegura de que el heap comience en una direccion alineada segun BYTE_ALIGNMENT*/
    alignedHeap = (uint8_t *) (((POINTER_SIZE_TYPE) & heap[BYTE_ALIGNMENT - 1]) & (~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK)));

    /* Se inicializa firstBlock. Marca el comienzo de la lista (tamaño 0) */
    firstBlock.nextBlock = (void *) alignedHeap;
    firstBlock.blockSize = (size_t) 0;

    /* Se inicializa lastBlock. Marca el final de la lista (tamaño maximo) */
    lastBlock.blockSize = ADJUSTED_HEAP_SIZE;
    lastBlock.nextBlock = NULL;

    /* Cuando se inicializa el heap por primera vez, solo existe un unico bloque
     * libre cuyo tamaño es maximo */
    firstFreeBlock = (void *) alignedHeap;
    firstFreeBlock->blockSize = ADJUSTED_HEAP_SIZE;
    firstFreeBlock->nextBlock = &lastBlock;
}

/* Inserta un bloque en la lista de bloques libres */
static void insertBlockIntoFreeList(memoryBlock_t * blockToInsert) {
    memoryBlock_t * blockIterator;
    size_t size = blockToInsert->blockSize;
    
    /* La lista de bloques libres se ordena de menor a mayor tamaño. Entonces,
    *  se itera por la lista hasta encontrar un bloque cuyo tamaño sea mayor al
    *  tamaño del bloque a insertar */
    for(blockIterator = &firstBlock; blockIterator->nextBlock->blockSize < size; blockIterator = blockIterator->nextBlock);

    /* Insertamos el bloque en la lista */ 
    blockToInsert->nextBlock = blockIterator->nextBlock;
    blockIterator->nextBlock = blockToInsert;
}

void * malloc(size_t wantedSize){
    memoryBlock_t * currentBlock;
    memoryBlock_t * previousBlock;
    memoryBlock_t * newBlock;
    void * retPointer = NULL;
    size_t aditionalRequiredSize;

    /* En la primera llamada a malloc, se inicializa el heap */
    if(wasHeapInitialized == FALSE){
        initializeHeap();
        wasHeapInitialized = TRUE;
    }
    
    if(wantedSize <= 0)
        return NULL;

    /* Al tamaño que pida el usuario, hay que sumarle el tamaño de un bloque de memoria
     * (que sera el bloque que represente a la memoria que sera entregada y se almacenara
     * contiguo a la memoria enregada) y alinearlo */
    aditionalRequiredSize = MemoryBlockStructSize + BYTE_ALIGNMENT - (wantedSize & BYTE_ALIGNMENT_MASK);

    /* Chequea si se puede incrementar wantedSize sin overflow */
    if(!ADD_WILL_OVERFLOW(wantedSize, aditionalRequiredSize))
        wantedSize += aditionalRequiredSize;
    else
        wantedSize = 0;

    /* Chequea si el wantedSize final es valido y busca un bloque apropiado */
    if(BLOCK_SIZE_IS_VALID(wantedSize) && (wantedSize > 0) && (wantedSize <= freeBytesRemaining)) {

        previousBlock = &firstBlock;
        currentBlock = firstBlock.nextBlock;

        /* Itero por los bloques hasta hallar uno de tamaño adecuado */
        while((currentBlock->blockSize < wantedSize) && (currentBlock->nextBlock != NULL)) {
            previousBlock = currentBlock;
            currentBlock = currentBlock->nextBlock;
        }
            
        /* Si al iterar llegue al ultimo bloque, entonces no existe un bloque de tamaño adecuado */
        if(currentBlock != &lastBlock){
            /* Al comienzo de esta direccion a retornar, guardaremos un memoryBlock_t que la represente.
             * por eso, en retPointer debemos "saltearla" */
            retPointer = (void *) (((uint8_t *) previousBlock->nextBlock) + MemoryBlockStructSize);

            /* Saco este bloque de la lista de libres */
            previousBlock->nextBlock = currentBlock->nextBlock;

            /* Si el bloque es mas grande de lo requerido, puede partirse en dos y el
             * fragmento sobrante reinsertarse como un bloque libre */
            if((currentBlock->blockSize - wantedSize) > MINIMUM_BLOCK_SIZE) {   

                /* Creo un nuevo bloque inmediatamente despues de currentBlock */
                newBlock = (void *) (((uint8_t *) currentBlock) + wantedSize);

                /* El tamaño de este nuevo bloque, sera la diferencia entre el tamaño original
                 * de currentBlock y el tamaño final que utilizara el usuario */
                newBlock->blockSize = currentBlock->blockSize - wantedSize;

                /* Entonces, ahora el tamaño de currentBlock sera exactamente wantedSize */
                currentBlock->blockSize = wantedSize;

                /* Inserto el nuevo bloque en la lista de libres */
                insertBlockIntoFreeList(newBlock);
            }

            freeBytesRemaining -= currentBlock->blockSize;

            /* Marco el bloque como usado y lo termino de desvincular de la lista */
            ALLOCATE_BLOCK(currentBlock);
            currentBlock->nextBlock = NULL;
        }
    }
    
    if(retPointer != NULL){
        // printf("\tSe entregara el puntero %p\n", retPointer);
        // printf("\t--- FBR: %ld (Se deben %ld)\n", freeBytesRemaining, ADJUSTED_HEAP_SIZE - freeBytesRemaining);
    }

    process * currentP = getExecutingP();
    for (int i = 0; i < 10; i++)
    {
        if(currentP->pc.allocated[i] == NULL){
            currentP->pc.allocated[i] = retPointer;
            //ncPrintHex((uint64_t)currentP->pc.allocated[i]);
            break;
        }

    }
    
    
    return retPointer;
}

void free(void * p) {
    uint8_t * blockPointer = (uint8_t *) p;
    memoryBlock_t * toFree;

    if(p != NULL) {
        /* El puntero que se le entrego al usuario, era contiguo a un memoryBlock_t que
        * representaba al bloque de memoria entregado. Al restar el tamaño de la estructura,
        * podemos posicionarnos al comienzo del mismo. */
        blockPointer -= MemoryBlockStructSize;

        toFree = (void *) blockPointer;

        if(BLOCK_IS_ALLOCATED(toFree) && toFree->nextBlock == NULL) {

            /* Marco el bloque como libre */
            FREE_BLOCK(toFree);
                
            /* Añado el bloque a la lista de libres */
            insertBlockIntoFreeList(((memoryBlock_t *) toFree));
            freeBytesRemaining += toFree->blockSize;
        }
    }
}

/* Funcion que puede usarse externamente para saber cuantos bytes libres le quedan al heap */
size_t getFreeHeapSize() {
    return freeBytesRemaining;
}

void mem(){

    print("Memoria total: ");
    printDec(HEAP_MAX_SIZE);
    print(" bytes\n");

    print("Memoria en uso: ");
    printDec(HEAP_MAX_SIZE - freeBytesRemaining);
    print(" bytes\n");

    print("Memoria libre: ");
    printDec(freeBytesRemaining);
    print(" bytes\n");
}
#endif

