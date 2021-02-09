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
int lineContains(const char* str, const char* sfind, const unsigned int length);
float extractFloat(const uint8_t* text, const unsigned int length);
int extractInteger(const uint8_t* text, const unsigned int length);
double extractDouble(const uint8_t* text, const unsigned int length);
bool extractState(const uint8_t* text, const unsigned int length);
bool extractBool(const uint8_t* text, const unsigned int length);
RGB extractRGB(const uint8_t* text, const unsigned int length);
