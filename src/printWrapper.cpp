
#ifdef ARDUINO
#include "Arduino.h"

void print(char msg)
{
}

void print(char* msg)
{
}

void print(const __FlashStringHelper* string)
{
}

void print(const __FlashStringHelper* string, char* msg1)
{
}

void print(const __FlashStringHelper* string, char* msg1, char* msg2)
{
}

void print(const __FlashStringHelper* string, int* num)
{
}

void print(const __FlashStringHelper* string, float* num)
{
}

#endif

#ifdef LINUX
#include "logger.hpp"

void print(char msg)
{
    debugMessage("{}", msg);
}

void print(char* msg)
{
    debugMessage("{}", msg);
}

void print(char* msg, char* msg1)
{
    debugMessage("{}", msg);
}

void print(char* msg, char* msg1, char* msg1)
{
}

void print(char* msg, int* num)
{
    debugMessage("{}", msg);
}

void print(char* msg, float* num)
{
    debugMessage("{}", msg);
}

#endif