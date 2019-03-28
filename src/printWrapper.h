#pragma once

#ifdef ARDUINO

void print(const char msg);
void print(const char* msg);
void print(const __FlashStringHelper* msg);
void print(const __FlashStringHelper* msg, const char* msg1);
void print(const __FlashStringHelper* msg, const char* msg1, const char* msg2);
void print(const __FlashStringHelper* msg, int num);
void print(const __FlashStringHelper* msg, float num);
#endif

#ifdef LINUX
// this will allow us to still save strings in flash in arduino
#define F(x) x

void print(const char msg);
void print(const char* msg);
void print(const char* msg, const char* msg1);
void print(const char* msg, const char* msg1, const char* msg2);
void print(const char* msg, int num);
void print(const char* msg, float num);
#endif
