#pragma once

#include "Arduino.h"

struct RGB
{
  int r;
  int g;
  int b;
};

void clearString(byte* text, unsigned int length);
int lineContains(const char *str, const char *sfind);
float extractFloat(byte* text, unsigned int length);
int extractInteger(byte* text, unsigned int length);
double extractDouble(byte* text, unsigned int length);
bool extractState(byte* text, unsigned int length);
bool extractBool(byte* text, unsigned int length);
RGB extractRGB(byte* text, unsigned int length);
