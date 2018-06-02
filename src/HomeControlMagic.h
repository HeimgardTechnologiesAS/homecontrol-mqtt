#pragma once

#include "Endpoint.h"
#include "NetworkObject.hpp"
#include "PubSubClient.h"

class HomeControlMagic
{
  public:
    HomeControlMagic(char* server_ip, char* deviceName, NetworkObject& network_object, char* username = "hc", char* password = "magic");
    void doMagic();

    Endpoint* getEndpoint(uint8_t number);
    void addEndpoint(Endpoint* endpoint_ptr);

    char* getId();
    uint8_t getNumberOfEndpoints();

    void sendConfigs();
    void sendStatus();
    void sendFeedback();

    void announce();

    void sendMessage(char* topic, char* message, char* endpoint_id);
    void sendMessage(char* topic, bool message, char* endpoint_id);
    void sendMessage(char* topic, uint16_t message, char* endpoint_id);
    void sendMessage(char* topic, double message, char* endpoint_id);

    void setReconnectTime(uint16_t seconds);

  private:
    bool reconnectMqtt();
    void subscribeNow();
    void mqttLoop(bool reconnect);

    char* m_name;

    NetworkObject& m_network_object;
    PubSubClient& m_mqtt_client;

    uint8_t m_number_of_endpoints;
    Endpoint* m_endpoints_pointers[10];

    const uint16_t m_mqtt_port = 1883;
    char* m_id;

    int m_reconnect_time = 5000;
    long m_last_time_connected = 0;
    long m_last_reconnect_attempt = 0;
    long m_led_time;
    long m_last_loop_time;

    char* m_username;
    char* m_password;

    char m_base_topic[40];
    bool m_broker_connected;

    bool m_start_done;
};

