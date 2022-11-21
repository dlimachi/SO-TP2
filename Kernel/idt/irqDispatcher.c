// This is a personal academic project. Dear PVS-Studio, please check it.
#ifndef IRQ_DISPATCHER
#define IRQ_DISPATCHER

#include <naiveConsole.h>
#include <keyboard_driver.h>
#include <timer_driver.h>
#include <syscalls.h>
#include <interrupts.h>
#include <irqDispatcher.h>

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	switch (irq) {
		case 0:
			int_20(); 								// Timer
			_sendEOI();
			break;
		 case 1:
		 	int_21((registerStruct *)registers); 	// Teclado
			_sendEOI();
	 		break;
		case 80: 
		int_80h((registerStruct*)registers); 
		_sendEOI();
    	break;
	}
	return;
}

void int_20() {
	timeHandler();
}

void int_21h(registerStruct * registers) {
  keyboardHandler(registers);
};

void int_80h(registerStruct * registers) {
  syscallHandler(registers);
}

#endif