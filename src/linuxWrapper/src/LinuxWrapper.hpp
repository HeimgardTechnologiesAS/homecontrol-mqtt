#pragma once

#include <string>

void wrapperLoop(bool reconnect = true);
void wrapperSetup();
void wrapperSetServer(std::string ip);
void wrapperSetCallback(void (*callback)(char*, uint8_t*, unsigned int));
bool wrapperIsMqttConnected();
void wrapperSetUsernamePassword(const char* const username, const char* const password);
void wrapperClearMessageBuffer();
void wrapperClearTopicBuffer();
char* wrapperGetTopicBuffer();
char* wrapperGetMessageBuffer();
void wrapperPublish();

// additional functions:
void dtostrf(double number, int left_of_decimal, int decimal_places, char* buffer);
char* getUniqueId();
void itoa(int value, char* str, int base);
uint32_t millis();