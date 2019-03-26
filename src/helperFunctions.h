#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#elif LINUX
#include <stdint.h>
#endif

struct RGB
{
    int r;
    int g;
    int b;
};

void clearBuffer(char* text, uint8_t length);
void clearByte(uint8_t* text, unsigned int length);
int lineContains(const char* str, const char* sfind);
float extractFloat(uint8_t* text, unsigned int length);
int extractInteger(uint8_t* text, unsigned int length);
double extractDouble(uint8_t* text, unsigned int length);
bool extractState(uint8_t* text, unsigned int length);
bool extractBool(uint8_t* text, unsigned int length);
RGB extractRGB(uint8_t* text, unsigned int length);
