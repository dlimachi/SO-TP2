#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <idtLoader.h>
#include <naiveConsole.h>
#include <lib.h>
#include <scheduler.h>
#include <inforeg.h>

extern int keyboardActivated();
extern unsigned char getPressedKey();

int keyboard_handler(uint64_t * registers);
uint64_t readBuffer(char* output, uint64_t count);

#endif 