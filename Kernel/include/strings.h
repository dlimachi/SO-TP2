#ifndef _STRINGS_H
#define _STRINGS_H

/**
 * Compara s1 y s2 y devuelve 0 si son iguales.
**/
int strcmp(const char * s1, const char * s2);

/**
 * Devuelve la longitud del string str
**/
unsigned int strlen(const char * str);

/**
 * "String Tokenizer": Divide el string pasado en 'input' en tokens segun
 * el caracter delimitador 'denim' y las guarda en 'tokenArray'. Acepta
 * hasta 'maxTokens' tokens y devuelve la cantidad de tokens en las que
 * se dividio el input.
**/
int strtok(char * input, char delim, char * tokenArray[], int maxTokens);

/**
 * Convierte el entero 'num' a string y lo guarda en 'str'
**/
void itoa(int num, char *str);

/**
 * Transforma el string 's' que representa un numero en formato hexadecimal
 * en un entero (base 10) y lo devuelve
**/
unsigned long hexaStringToInt(char *s);

/**
 * Transforma el caracter 'c' en su version mayuscula en caso de que este
 * se una letra minuscula
**/
char convertCharToUpperCase(char c);

#endif