#pragma once

#ifdef ESP32
#include "WiFi.h"

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
#endif // ESP8266
