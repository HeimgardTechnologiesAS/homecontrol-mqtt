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

    Endpoint* getEndpoint(uint8_t number);
    void addEndpoint(Endpoint* endpoint_ptr);

    char* getId();
    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendStatus();
    void sendFeedback();

    void announce();

    void sendStringMessage(char* topic, char* endpoint_id);
    void sendMessage(char* topic, bool message, char* endpoint_id);
    void sendMessage(char* topic, uint16_t message, char* endpoint_id);
    void sendMessage(char* topic, double message, char* endpoint_id);

    void sendConfig(char* config, uint8_t, char* endpoint_name, char* endpoint_id);

    char* getMessageBufferPtr();

  private:
    void setTopic(char* topic, char* endpoint_id);

    const char* m_name;
    char* m_id;

    uint8_t m_number_of_endpoints;
    Endpoint* m_endpoints_pointers[10];  
    char m_base_topic[20];
    bool m_broker_was_connected;
};

