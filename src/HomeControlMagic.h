#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#endif

#include "Endpoint.h"

class HomeControlMagic
{
public:
    HomeControlMagic(const char* deviceName);
    void doMagic();
    void setup();

    Endpoint* getEndpoint(char* endpoint_id);
    void addEndpoint(Endpoint* endpoint_ptr);
    void addEndpoint(Endpoint* endpoint_ptr, char* endpoint_id);

    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendFeedback();

    void announce();

    void sendStringMessage(char* topic, char* endpoint_id);
    void sendMessage(char* topic, bool message, char* endpoint_id);
    void sendMessage(char* topic, uint16_t message, char* endpoint_id);
    void sendMessage(char* topic, double message, char* endpoint_id);

    void sendConfig(char* config, char* endpoint_name, char* endpoint_id);

    char* getMessageBufferPtr();

private:
    void setTopic(char* topic, char* endpoint_id);

    const char* m_name;
    const char* m_id;

    uint8_t m_number_of_endpoints;
    Endpoint* m_endpoints_pointers[50];
    char m_base_topic[40];
    bool m_broker_was_connected;
};
