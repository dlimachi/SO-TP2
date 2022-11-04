#ifndef LOOP_H
#define LOOP_H

#include <stdio.h>
#include <strings.h>
#include <phylo.h>
#include <syscall.h>

void sleep(int argSize, char *args[]);

void loop();

void cat();

void wc();

void filter();

int isVowel(char c);

void testMalloc();

#endif