#include "mqtt.hpp"
#include "logger.hpp"

namespace mqtt
{
Mqtt::Mqtt(const char* client_id, std::string ip_string)
    : mosquittopp(client_id)
    , id(client_id)
    , host(ip_string.c_str())
    , port(1883)
    , keepalive(120)
{
    debugMessage("Constructor");
    mosqpp::lib_init(); // Initialize libmosquitto

    username_pw_set("hc", "magic");

    debugMessage("Host for mqtt broker is: {}", host);
    // non blocking connection to broker request
    connect_async(host, port, keepalive);
    loop_start(); // Start thread managing connection / publish / subscribe
}

Mqtt::~Mqtt()
{
    loop_stop();           // Kill the thread
    mosqpp::lib_cleanup(); // Mosquitto library cleanup
}

void Mqtt::on_connect(int rc)
{
    debugMessage("Connected with status: {}.", mosqpp::connack_string(rc));

    if(rc == 0)
    {
        subscribe(NULL, "d/#");
        sendMessage("broadcast", "serverannounce");
    }
}

void Mqtt::on_disconnect(int rc)
{
    debugMessage("Disconnected with status: {}.", mosqpp::connack_string(rc));
}

void Mqtt::on_subscribe(int mid, int qos_count, const int* granted_qos)
{
    debugMessage("Subscription succeeded. \n");
}

void Mqtt::on_message(const struct mosquitto_message* message)
{
    std::string topic = message->topic;
    uint8_t* data_ptr = (uint8_t*)message->payload;
    // MessageArray message_vector(data_ptr, data_ptr + message->payloadlen);

    // TODO: remove this

    /*debugMessage("got message on topic: {}", topic);
    if(hc::Logger::isDebugLogging())
    {
        for(uint8_t c : message_vector)
        {
            printf("%c", (char)c);
        }
        printf("\n");
    }

    m_ios.post(
        std::bind(&DeviceManager::incomingMessage, m_device_manager_ptr, std::move(topic), std::move(message_vector)));
*/
}

void Mqtt::on_publish(int mid)
{
    // TODO: use message id to confirm send messages
    // std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}

void Mqtt::sendMessage(std::string topic, std::string message)
{
    debugMessage("Sending message: {}; on topic: {}", message, topic);
    int ret = publish(NULL, topic.c_str(), strlen(message.c_str()), message.c_str(), 1, false);
}

} // namespace mqtt