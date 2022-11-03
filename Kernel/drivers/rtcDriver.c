// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <rtcDriver.h>

uint8_t getTime(uint64_t descriptor){
    return getRTC(descriptor);
}

uint64_t getDecis(){
    return deciseconds_elapsed();
}

uint64_t getSeconds(){
    return seconds_elapsed();
}