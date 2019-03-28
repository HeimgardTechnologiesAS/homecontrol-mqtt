
#ifdef ARDUINO
#include "Arduino.h"

void print(const char msg)
{
    Serial.println(msg);
}

void print(const char* msg)
{
    Serial.println(msg);
}

void print(const __FlashStringHelper* msg)
{
    Serial.println(msg);
}

void print(const __FlashStringHelper* msg, const char* msg1)
{
    Serial.print(msg);
    Serial.println(msg1);
}

void print(const __FlashStringHelper* msg, const char* msg1, const char* msg2)
{
    Serial.print(msg);
    Serial.print(msg1);
    Serial.println(msg2);
}

void print(const __FlashStringHelper* msg, int num)
{
    Serial.print(msg);
    Serial.println(num);
}

void print(const __FlashStringHelper* msg, float num)
{
    Serial.print(msg);
    Serial.println(num);
}

#endif

#ifdef LINUX
#include "logger.hpp"

void print(const char msg)
{
    debugMessage("{}", msg);
}

void print(const char* msg)
{
    debugMessage("{}", msg);
}

void print(const char* msg, const char* msg1)
{
    debugMessage("{} {}", msg, msg1);
}

void print(const char* msg, const char* msg1, const char* msg2)
{
    debugMessage("{} {} {}", msg, msg1, msg2);
}

void print(const char* msg, int num)
{
    debugMessage("{} {}", msg, num);
}

void print(const char* msg, float num)
{
    debugMessage("{} {}", msg, num);
}

#endif