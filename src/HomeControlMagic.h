#pragma once

#include "Endpoint.h"
#include "LoopObject.hpp"
#include "PubSubClient.h"

class HomeControlMagic
{
  public:
    HomeControlMagic(char* server_ip, const String deviceName, LoopObject& network_object);
    void doMagic();

    Endpoint* getEndpoint(uint8_t number);
    void addEndpoint(Endpoint* endpoint_ptr);

    String getId();
    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendStatus();
    void announce();

    void sendMessage(String topic, String message, char* endpoint_id);
    void sendMessage(String topic, uint16_t message, char* endpoint_id);

  private:
    bool reconnectMqtt();
    void subscribeNow();
    void mqttLoop(bool reconnect);

    String m_name;

    LoopObject& m_network_object;
    PubSubClient& m_esp_client;

    uint8_t m_number_of_endpoints;
    Endpoint* m_endpoints_pointers[10];

    const uint16_t m_mqtt_port = 1883;
    String m_id;

    int m_reconnectTime = 30000;
    long m_lastTimeConnected = 0;
    long m_lastReconnectAttempt = 0;

    char m_base_topic[20];
};

