#include <stdint.h>
#include <strings.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <exceptions.h>
#include <idtLoader.h>
#include <scheduler.h>
#include <interrupts.h>
#include <semaphores.h>
#include <pipe.h>
#include <IOManager.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
/*	ncClear();
    // TODO decide actual location and size of heap
    createMemoryManager((void*)0x900000, 16384);
    initializeScheduler();
    char *argv[] = {"bash"};
    createProcess((void (*)(int, char **))sampleCodeModuleAddress, 1, (char **) &argv);
	load_idt();
    forceTimerTick();
//	loadUserland(sampleCodeModuleAddress, (uint64_t*) 0x900000);
	ncPrint("[Finished]");
	return 0;
	*/
	load_idt();
	ncClear();

	_cli();
	initSemaphores();
	_sti();

	_cli();
	initPipes();
	_sti();

	_cli();
	initScheduler();
	_sti();

	saveInitialState((uint64_t)sampleCodeModuleAddress, getSP());
	
	((EntryPoint)sampleCodeModuleAddress)();
	/* No vuelve a este punto despues de que se borra firstProcess */
	while(1);
	
	return 0;
}
