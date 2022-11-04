#ifndef _PHYLO_H
#define _PHYLO_H

#include <strings.h>
#include <stdio.h>
#include <syscall.h>

#define INITIAL_PHYL 5
#define MAX_PHYL 10

#define ROOM_SEM_NAME "room"
#define EATING_CHAR 'E'
#define EXITED_CHAR 'X'
#define WAITING_CHAR '.'
#define ADD_PHILO_KEY 'a'
#define REMOVE_PHILO_KEY 'd'


void phylo_main();

#endif