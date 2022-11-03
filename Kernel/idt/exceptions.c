// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <exceptions.h>

static initialState initS;

void exceptionDispatcher(int exception, uint64_t *registers) {
	switch(exception){
		case ZERO_EXCEPTION_ID:
			zeroDivision();
			break;
		case INVALID_OPCODE_EXCEPTION_ID:
			invalidOpcode();
			break;
		default:
			return;
	}
	saveRegisterInfo();
	getRegistersInfo();

	restartTerminal(registers);
}

static void printRegisters(uint64_t rip, uint64_t rsp, const uint64_t * backup){
	ncNewline();
	ncPrintFormat("RIP:", ERROR_FORMAT);
	ncPrintHex(rip);
	ncPrintFormat(" RSP:", ERROR_FORMAT);
	ncPrintHex(rsp);
	for (int i = 14; i >= 0; i--)
	{
		ncPrintFormat(registers[14-i], ERROR_FORMAT);
		ncPrintHex(backup[i]);
	}
}

void zeroDivision() {
	ncPrintFormat("ERROR: Zero division.", ERROR_FORMAT);
}

void invalidOpcode() {
	ncPrintFormat("ERROR: Invalid operation code.", ERROR_FORMAT);
}

void saveInitialState(uint64_t IP, uint64_t SP){
	initS.IP = IP;
	initS.SP = SP;
}

void restartTerminal(uint64_t *registers){
	registers[IP_INDEX] = initS.IP;
	registers[SP_INDEX] = initS.SP;
}