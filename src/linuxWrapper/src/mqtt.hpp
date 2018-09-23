/*
#pragma once

#include <mosquittopp.h>
#include <thread>

namespace hc
{
namespace mqtt
{
class DeviceManager;

class Mqtt : public mosqpp::mosquittopp
{
public:
    Mqtt(const char* driver_id, asio::io_service& ios, DeviceManager* device_manager, std::string ip_string);
    ~Mqtt();

    void sendMessage(std::string topic, std::string message);

    // this are overloaded functions
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_message(const struct mosquitto_message* message);
    void on_subcribe(int mid, int qos_count, const int* granted_qos);
    void on_publish(int mid);

private:
    const char* host;
    const char* id;
    const char* topic;
    int port;
    int keepalive;
};

} // namespace mqtt
} // namespace hc*/