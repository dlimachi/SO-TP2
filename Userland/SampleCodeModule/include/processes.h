#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdio.h>
#include <strings.h>
#include <phylo.h>
#include <syscall.h>


int isVowel(char c);

void sleep(int argSize, char *args[]);

void loop();

void cat();

void wc();

void filter();

void ps(uint64_t argc, char *argv[]);

void pipe(uint64_t argc, char *argv[]);

void block(uint64_t argc, char *argv[]);

void nice(uint64_t argc, char *argv[]);

#endif