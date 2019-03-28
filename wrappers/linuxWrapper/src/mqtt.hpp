#pragma once

#include <atomic>
#include <mosquittopp.h>
#include <thread>

#define MESSAGE_BUFFER_SIZE 250
#define TOPIC_BUFFER_SIZE 250

namespace mqtt
{
class Mqtt : public mosqpp::mosquittopp
{
public:
    Mqtt(const char* client_id,
         const std::string& gw_ip,
         const std::string& username,
         const std::string& password,
         const bool is_secure);
    ~Mqtt();

    void sendMessage();
    bool isConnected();
    bool isSecure();

    // this are overloaded functions
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_message(const struct mosquitto_message* message);
    void on_subscribe(int mid, int qos_count, const int* granted_qos);
    void on_publish(int mid);

    void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
    char* getMessageBuffer();
    char* getTopicBuffer();
    void clearMessageBuffer();
    void clearTopicBuffer();

private:
    void fetchCertificate();

    const char* host;
    const char* id;
    const char* topic;
    int port;
    int keepalive;
    std::atomic<bool> m_connected;
    bool m_is_secure;

    char m_message_buffer[MESSAGE_BUFFER_SIZE];
    char m_topic_buffer[TOPIC_BUFFER_SIZE];

    // function pointer to function in HomeControlMagic object
    void (*m_callback)(char*, uint8_t*, unsigned int);
};

} // namespace mqtt