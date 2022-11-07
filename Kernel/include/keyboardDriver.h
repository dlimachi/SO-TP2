#ifndef __keyboardDriver_H_
#define __keyboardDriver_H_

#include <naiveConsole.h>

/**
 * Devuelve verdadero mientras el teclado este activado
**/
extern int keyboardActivated();

/**
 * Devuelve el scancode de la tecla presionada
**/
extern unsigned char getPressedKey();

/**
 * Cuando se presiona una tecla, se genera una interrupcion que llama a 
 * esta funcion. Se guarda el ascii de la tecla en el buffer.
**/
int keyboardHandler();

/**
 * Devuelve el scancode de la tecla presionada
**/
unsigned char scancodeToAscii(int scancode);

/**
 *  Devuelve el primer caracter del buffer
**/
unsigned char kb_getChar();


#endif