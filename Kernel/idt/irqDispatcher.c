// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <irqDispatcher.h>

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	switch (irq) {
		case 0:
			int_20(); 			// Timer
			break;
		 case 1:
		 	int_21(registers); 			// Teclado
	 		break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(uint64_t * registers){
	keyboardHandler(registers);
}