#pragma once

#include "Endpoint.h"
#include "LoopObject.hpp"
#include "PubSubClient.h"

class HomeControlMagic
{
  public:
    HomeControlMagic(char* server_ip, const String deviceName, LoopObject& network_object, String username = "hc", String password = "magic");
    void doMagic();

    Endpoint* getEndpoint(uint8_t number);
    void addEndpoint(Endpoint* endpoint_ptr);

    String getId();
    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendStatus();
    void announce();

    void sendMessage(String topic, String message, char* endpoint_id);
    void sendMessage(String topic, bool message, char* endpoint_id);
    void sendMessage(String topic, uint16_t message, char* endpoint_id);
    void sendMessage(String topic, double message, char* endpoint_id);

    void setReconnectTime(uint16_t seconds);

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

    int m_reconnect_time = 5000;
    long m_last_time_connected = 0;
    long m_last_reconnect_attempt = 0;
    long m_led_time;
    long m_last_loop_time;

    String m_username;
    String m_password;

    char m_base_topic[20];
};

