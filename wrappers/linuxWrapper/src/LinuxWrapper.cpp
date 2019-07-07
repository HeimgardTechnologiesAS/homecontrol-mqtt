#include "LinuxWrapper.hpp"
#include "logger.hpp"
#include "mqtt.hpp"

#include <fstream>
#include <sys/time.h>

static std::string m_uid = "";

static mqtt::Mqtt* mqtt_ptr = nullptr;

void linuxSetMqttPtr(mqtt::Mqtt* mqtt)
{
    mqtt_ptr = mqtt;
}

void wrapperLoop(bool reconnect)
{
    // not used here
}

void wrapperSetup(const std::string& custom_uid)
{
    // get uid
    if(custom_uid != "")
    {
        infoMessage("Setting custom uid {}", custom_uid);
        m_uid = custom_uid;
    }
    else
    {
        std::ifstream uid_file("/etc/machine-id");
        if(uid_file.is_open())
        {
            // std::getline(uid_file, m_uid);
            char c = 0;
            while(uid_file.get(c))
            {
                if(c < 48)
                {
                    c = 48;
                }
                else if(c > 122)
                {
                    c = 122;
                }
                m_uid += c;
            }
        }
        else
        {
            // TODO: think of something else
        }
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
    return mqtt_ptr->isConnected();
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

bool networkIsSecure()
{
    return mqtt_ptr->isSecure();
}

// additional functions:
void dtostrf(double number, int left_of_decimal, int decimal_places, char* buffer)
{
    std::string string;
    string = fmt::format("{:.{}f}", number, decimal_places);
    strcpy(buffer, string.c_str());
}

const char* getUniqueId()
{
    return m_uid.c_str();
}

void itoa(int value, char* str, int base)
{
    if(base != 10)
    {
        errorMessage("Just use base 10");
        return;
    }

    std::string s = std::to_string(value);
    strcpy(str, s.c_str());
}

uint32_t millis()
{
    struct timeval te;
    gettimeofday(&te, NULL);                                        // get current time
    uint32_t milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}