// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <inforeg.h>

static uint64_t genRegs[REG_AMOUNT+3] = {0};
static uint8_t set = 0;

void setRegisters(uint64_t * registers){
    int i = 0;
    for (; i < REG_AMOUNT; i++)
    {
        genRegs[i] = registers[i];
    }
    genRegs[i++] = registers[RIP_POS];
    genRegs[i++] = registers[RFLAGS_POS];
    genRegs[i] = registers[RSP_POS];
    set = 1;
}

void getRegisters(){
    if(set == 0){
        return;
    }

    char * regs[REG_AMOUNT] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RSP", "RIP"};
    
    char str[8] = {'0'};
    for(int i = 0; i < REG_AMOUNT; i++){
        ncPrint(regs[i]);
        ncPrint(": ");

        uint32_t dig = uintToBase(backup[i], str, REG_AMOUNT);
        for(int i=0; i < 8-dig; i++){
           ncPrintChar('0');
        } 
        
        ncPrint(str);
        ncPrintChar('\n');
    }
}

void printMemory(uint64_t argument){

    char dump[32] = {'0'};

    uint8_t *memAddress = (uint8_t *)argument;
    
    for (int i = 0; i < 32; i++){
        uint32_t dig = uintToBase(memAddress[i], dump, 16);
        for(int i=0; i < 2-dig; i++){
           ncPrintChar('0');
       }
        ncPrint(dump);
        ncPrintChar(' ');
    }
    
    ncPrintChar('\n');
}