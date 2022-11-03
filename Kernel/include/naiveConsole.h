#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define TOTAL_SCREEN_LENGTH width*height*2
#define LINE_LENGTH width*2
#define COL_LENGTH height*2
#define LAST_LINE video + TOTAL_SCREEN_LENGTH - LINE_LENGTH
#define DEFAULT_COLOR WHITE_BLACK
#define PROMPT_SIZE 5 // chars que ocupa el prompt

#define TAB "    "

#define ERROR_FORMAT 0x0C
#define RED_BLACK 0x04

void ncDeleteChar();
void ncPrint(const char * string);
void ncPrintChar(char character);
void ncPrintInPos(const char * string, int row, int col);
void ncPrintCharInPos(char c, int row, int col);
void ncPrintFormat(const char* string,uint8_t format);
void ncPrintCharFormat(char character,uint8_t format);
void ncClear();
void ncScrollUp();
void ncBackspace();
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClearWindow(uint8_t windowToCLear);
uint8_t ncWindows(uint8_t amount);
uint8_t ncCurrentWindow(uint8_t select);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif