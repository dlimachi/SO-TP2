// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <mystdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_util.h>
#include <mysyscall.h>
#include <syscall.h>

#define MINOR_WAIT 1000000 
#define WAIT      100000000 
#define SUPERWAIT WAIT*2 

#define TOTAL_PROCESSES 3
#define LOWEST 19 
#define MEDIUM 10 
#define HIGHEST 0 

int count1=0;
int count2=0;
int count3=0;

char prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};
int64_t pids[TOTAL_PROCESSES];

void endless_loop_print(){
  int64_t pid = sys_getPid();
  int wait = 100000;
  while(1){
    printInt(pid);
    if(pid == pids[0])
      count1++;
    else if(pid == pids[1])
      count2++;
    else
      count3++;
    bussy_wait(wait);
  }
}

void test_prio(){

  char *argv[] = {"endless_loop_print"};
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
      pids[i] = sys_createProcess(&endless_loop_print, 1, argv, NULL, BACKGROUND);
  

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    sys_nice(pids[i], prio[i]);
  }

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    sys_nice(pids[i], MEDIUM);
  }

  print("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblock(pids[i]);

  bussy_wait(SUPERWAIT);
  print("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    sys_killPs(pids[i]);
  
  print("Count p1: ");
  printInt(count1);
  print("\nCount p2: ");
  printInt(count2);
  print("\nCount p3: ");
  printInt(count3);

  print("\nDONE\n");
  
}
