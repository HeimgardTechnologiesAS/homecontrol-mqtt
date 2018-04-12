#pragma once

#include "Arduino.h"

void clearString(byte* text, unsigned int length);
int lineContains(const char *str, const char *sfind);
float extractFloat(byte* text, unsigned int length);
int extractInteger(byte* text, unsigned int length);
double extractDouble(byte* text, unsigned int length);
bool extractState(byte* text, unsigned int length);
bool extractBool(byte* text, unsigned int length);
