// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <bash.h>

char * v = (char*)0xB8000 + 79 * 2;

int main() {
	char *argv[] = {"Terminal"};
	sys_createProcess(&startTerminal, 1, argv, NULL, FOREGROUND);
	return 0;
}