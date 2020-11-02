#pragma once

#include <string>

namespace mqtt
{
class Mqtt;
}

void wrapperLoop(bool reconnect = true);
void wrapperSetup(const std::string& custom_uid = "");
void wrapperSetServer(std::string ip);
void wrapperSetCallback(void (*callback)(char*, uint8_t*, unsigned int));
bool wrapperIsMqttConnected();
void wrapperSetUsernamePassword(const char* const username, const char* const password);
void wrapperClearMessageBuffer();
void wrapperClearTopicBuffer();
char* wrapperGetTopicBuffer();
char* wrapperGetMessageBuffer();
void wrapperPublish();

bool networkIsSecure();

void linuxSetMqttPtr(mqtt::Mqtt* mqtt);

// additional functions:
void dtostrf(double number, int left_of_decimal, int decimal_places, char* buffer);
const char* getUniqueId();
void itoa(int value, char* str, int base);
