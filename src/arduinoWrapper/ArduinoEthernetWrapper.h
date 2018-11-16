#pragma once

#include "ArduinoConfig.h"

#if defined(ETHERNET) || defined(ETHERNET2)

#ifdef ETHERNET
#include "Ethernet.h"
#elif defined(ETHERNET2)
#include "Ethernet2.h"
#endif

void networkLoop();
void networkSetup();
void networkStart();
void networkChipRestart();
Client& networkGetClient();
bool networkIsConnected();

void networkSetSecure(bool secure);
bool networkIsSecure();
void setReconnectTime(unsigned long reconnect_time);
void networkSetSsid(const char* const ssid);
void networkSetPass(const char* const pass);

char* getUniqueId();

#endif // defined(ETHERNET) || defined(ETHERNET2)