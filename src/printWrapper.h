#pragma once

#ifdef ARDUINO

void print(char msg);
void print(char* msg);
void print(const __FlashStringHelper* string);
void print(const __FlashStringHelper* string, char* msg1);
void print(const __FlashStringHelper* string, char* msg1, char* msg2);
void print(const __FlashStringHelper* string, int* num);
void print(const __FlashStringHelper* string, float* num);
#endif

#ifdef LINUX
// this will allow us to still save strings in flash in arduino
#define F(x) x

void print(char msg);
void print(char* msg);
void print(char* msg, char* msg1);
void print(char* msg, char* msg1, char* msg1);
void print(char* msg, int* num);
void print(char* msg, float* num);
#endif
