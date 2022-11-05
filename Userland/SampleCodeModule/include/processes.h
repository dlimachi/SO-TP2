#ifndef LOOP_H
#define LOOP_H

#include <mystdio.h>
#include <stdio.h>
#include <strings.h>
#include <phylo.h>
#include <mysyscall.h>

void sleep(int argSize, char *args[]);

void loop();

void cat();

void wc();

void filter();

int isVowel(char c);

void testMalloc();

#endif