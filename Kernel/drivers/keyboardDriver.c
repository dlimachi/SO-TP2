// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>

#define MAX_SIZE 1024
#define BACKSPACE 0x0E

static unsigned char buffer[MAX_SIZE];
static int writeIdx = 0;
static int readIdx = 0;

int keyboardHandler(){
    unsigned char scancodeKey = 0;
    while(keyboardActivated()){
        scancodeKey = getPressedKey();

        if(scancodeKey == BACKSPACE)
            ncBackspace();

        if(scancodeToAscii(scancodeKey) != 0){
            buffer[writeIdx] = scancodeToAscii(scancodeKey);
            writeIdx = (writeIdx+1) % MAX_SIZE;
            return 1;
        }
    }
    return 0;
}

// Fuente: https://stackoverflow.com/questions/61124564/convert-scancodes-to-ascii
unsigned char scancodeToAscii(int scancode){
        unsigned char kbd_US [128] ={
    0,  27, /* <-- Escape */
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
    '\t', /* <-- Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    200, /* <-- control key */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    201, /* Shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
    '*',
    202,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
    };

    return kbd_US[scancode];
}

unsigned char kb_getChar(){
    if(readIdx == writeIdx)
        return 0;
    unsigned char key = buffer[readIdx];
    readIdx = (readIdx + 1) % MAX_SIZE;
    return key;
}


