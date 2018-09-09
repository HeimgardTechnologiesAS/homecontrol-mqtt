#pragma once

#include "IPAddress.h"

void wrapperLoop(bool reconnect = true);
void wrapperSetup();
void wrapperSetServer(IPAddress ip);
void wrapperSetServer(uint8_t* ip);
void wrapperSetServer(char* ip);
void wrapperSetCallback(void (*callback)(char*, uint8_t*, unsigned int));
bool wrapperIsMqttConnected();
void wrapperSetUsernamePassword(const char* const username, const char* const password);
void wrapperClearMessageBuffer();
void wrapperClearTopicBuffer();
char* wrapperGetTopicBuffer();
char* wrapperGetMessageBuffer();
void wrapperPublish();