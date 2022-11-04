#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <stddef.h>
#include <lib.h>
#include <naiveConsole.h>

void loadModules(void * payloadStart, void ** moduleTargetAddress);

#endif