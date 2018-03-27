#pragma once

#include "Endpoint.h"

class HomeControlMagic
{
  public:
    HomeControlMagic(char* server_ip, const String deviceName);
    void doMagic(bool connection_ok);

    Endpoint* getEndpoint(uint8_t number);
    void addEndpoint(Endpoint* endpoint_ptr);

    char* getId();
    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendStatus();
    void announce();

    void sendMessage(String topic, String message, char* endpoint_id);
    void sendMessage(String topic, uint8_t message, char* endpoint_id);

  private:
    bool reconnectMqtt();
    void subscribeNow();
    void mqttLoop(bool reconnect);

    String m_name;


    uint8_t m_number_of_endpoints;
    Endpoint* m_endpoints_pointers[10];

    const uint16_t m_mqtt_port = 1883;
    char m_id[10];

    int m_reconnectTime = 30000;
    long m_lastTimeConnected = 0;
    long m_lastReconnectAttempt = 0;
};

