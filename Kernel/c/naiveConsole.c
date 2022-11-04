// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>

static char buffer[64] = {'0'};
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;



void ncPrint(const char * string){
	ncPrintWithColor(string, DEFAULT_COLOR);
}

void ncPrintWithColor(const char * string, uint8_t colorCode){
	if(currentVideo == LAST_LINE){
		ncScrollUp();
		currentVideo = LAST_LINE;
	}

	for (int i = 0; string[i] != 0; i++){
		if(string[i] == '\n')
			ncNewline();
		else
			ncPrintCharWithColor(string[i], colorCode);
	}
}

void ncPrintInPos(const char * string, int row, int col, uint8_t colorCode){
    for(int i = 0; string[i] != 0; i++) {
		if(col + i >= width)
			return;
		ncPrintCharInPos(string[i], row, col + i, colorCode);
	}
}

void ncPrintCharInPos(char c, int row, int col, uint8_t colorCode){
	if(row < 0 || row > height)
		return;
	if(col < 0 || col > width)
		return;

	uint8_t * printPos = video + row * LINE_LENGTH + col * 2;
	printPos[0] = c;
	printPos[1] = colorCode;
}

void ncPrintChar(char character){
	ncPrintCharWithColor(character, DEFAULT_COLOR);
}

void ncPrintCharWithColor(char character, uint8_t colorCode){
	if(currentVideo == LAST_LINE + LINE_LENGTH){
		ncScrollUp();
		currentVideo = LAST_LINE;
	}

	if(character == '\n')
		ncNewline();
	else {
		*currentVideo = character;
		currentVideo++;
		*currentVideo = colorCode;
		currentVideo++;
	}
}

void ncNewline(){
	do {
		ncPrintChar(' ');
	} while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncScrollUp(){
	int i;
	for (i = 0; i < TOTAL_SCREEN_LENGTH - LINE_LENGTH; i++) {
		video[i] = video[i + LINE_LENGTH];
	}
	
	while(i < TOTAL_SCREEN_LENGTH){
		if(i % 2 == 0)
			video[i++] = ' ';
		else
			video[i++] = DEFAULT_COLOR;
	}
}

void ncBackspace(){
	//Si estoy al principio de la linea (adelante del prompt)
	uint64_t posInLine = (uint64_t)(currentVideo - video) % (uint64_t)(LINE_LENGTH);
	if(posInLine <= PROMPT_SIZE + 1)
		return;

	currentVideo--;
	*currentVideo = DEFAULT_COLOR;
	currentVideo--;
	*currentVideo = ' ';
}

void ncPrintDec(uint64_t value){
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value){
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear(){
	int i;

	for (i = 0; i < height * width; i++){
		video[2*i] = ' ';
		video[2*i+1] = DEFAULT_COLOR;
	}

	currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
