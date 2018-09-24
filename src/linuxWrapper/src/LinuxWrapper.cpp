#include "LinuxWrapper.hpp"
#include "logger.hpp"
#include "mqtt.hpp"

#include <fstream>

static std::string m_uid;

static mqtt::Mqtt* mqtt_ptr = nullptr;

void wrapperLoop(bool reconnect)
{
    // not used here
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
    errorMessage("This is not used here. Set broker ip from main");
}

void wrapperSetCallback(void (*callback)(char*, uint8_t*, unsigned int))
{
    mqtt_ptr->setCallback(callback);
}

bool wrapperIsMqttConnected()
{
    mqtt_ptr->isConnected();
}

void wrapperSetUsernamePassword(const char* const username, const char* const password)
{
    errorMessage("This is not used here. Set username and password from main");
}

void wrapperClearMessageBuffer()
{
    mqtt_ptr->clearMessageBuffer();
}

void wrapperClearTopicBuffer()
{
    mqtt_ptr->clearTopicBuffer();
}

char* wrapperGetTopicBuffer()
{
    return mqtt_ptr->getTopicBuffer();
}

char* wrapperGetMessageBuffer()
{
    return mqtt_ptr->getMessageBuffer();
}

void wrapperPublish()
{
    mqtt_ptr->sendMessage();
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