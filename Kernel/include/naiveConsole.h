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

// Colores. El formato es COLORLETRA_COLORFONDO
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F


void ncPrint(const char * string);

void ncPrintChar(char character);

void ncPrintInPos(const char * string, int row, int col, uint8_t colorCode);

void ncPrintCharInPos(char c, int row, int col, uint8_t colorCode);

void ncPrintWithColor(const char * string, uint8_t colorCode);

void ncPrintCharWithColor(char character, uint8_t colorCode);

void ncClear();

void ncScrollUp();

void ncBackspace();

void ncNewline();

void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);



#endif