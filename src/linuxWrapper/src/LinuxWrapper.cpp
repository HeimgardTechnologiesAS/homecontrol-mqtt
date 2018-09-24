#include "LinuxWrapper.hpp"
#include "logger.hpp"

#include <fstream>

static std::string m_uid;

void wrapperLoop(bool reconnect)
{
}
void wrapperSetup()
{
    // get uid
    std::ifstream uid_file("/etc/machine-id");
    if(uid_file.is_open())
    {
        std::getline(uid_file, m_uid);
    }
    else
    {
        // TODO: think of something else
    }
    infoMessage("ID of this device: {}", m_uid);
}
void wrapperSetServer(std::string ip)
{
}
void wrapperSetCallback(void (*callback)(char*, uint8_t*, unsigned int))
{
}
bool wrapperIsMqttConnected()
{
}
void wrapperSetUsernamePassword(const char* const username, const char* const password)
{
}
void wrapperClearMessageBuffer()
{
}
void wrapperClearTopicBuffer()
{
}
char* wrapperGetTopicBuffer()
{
}
char* wrapperGetMessageBuffer()
{
}
void wrapperPublish()
{
}

// additional functions:
void dtostrf(double number, int left_of_decimal, int decimal_places, char* buffer)
{
}

char* getUniqueId()
{
    return (char*)m_uid.c_str();
}

void itoa(int value, char* str, int base)
{
}
uint32_t millis()
{
}