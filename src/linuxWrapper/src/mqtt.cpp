#include "mqtt.hpp"
#include "logger.hpp"

namespace mqtt
{
Mqtt::Mqtt(const char* client_id, std::string gw_ip, std::string username, std::string password, bool is_secure)
    : mosquittopp(client_id)
    , id(client_id)
    , host(gw_ip.c_str())
    , keepalive(120)
    , m_connected(false)
    , m_is_secure(is_secure)
{
    debugMessage("Constructor mqtt");
    mosqpp::lib_init(); // Initialize libmosquitto

    username_pw_set(username.c_str(), password.c_str());

    debugMessage("Host for mqtt broker is: {}", gw_ip);

    if(m_is_secure)
    {
        port = 8883;
        debugMessage("Using secure port");
    }
    else
    {
        port = 1883;
        debugMessage("Using unsecured port");
    }
    clearTopicBuffer();
    clearMessageBuffer();

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
        m_connected = true;
        // subsribe to device specific
        std::string subscribe_topic = std::string(id) + "/#";
        debugMessage("subscribe topic : {}", subscribe_topic);
        subscribe(NULL, subscribe_topic.c_str());
        // subscribe to broadcast
        subscribe_topic = "broadcast";
        debugMessage("subscribe topic : {}", subscribe_topic);
        subscribe(NULL, subscribe_topic.c_str());
    }
}

void Mqtt::on_disconnect(int rc)
{
    m_connected = false;
    debugMessage("Disconnected with status: {}.", mosqpp::connack_string(rc));
}

void Mqtt::on_subscribe(int mid, int qos_count, const int* granted_qos)
{
    debugMessage("Subscription succeeded. \n");
}

void Mqtt::on_message(const struct mosquitto_message* message)
{
    uint8_t* data_ptr = (uint8_t*)message->payload;
    if(logger.isDebugLoggingEnabled())
    {
        std::stringstream ss;
        for(int i = 0; i < message->payloadlen; i++)
        {
            ss << (char*)message->payload + i;
        }
        debugMessage("Received message: {}, on topic: {}", ss.str(), message->topic);
    }

    m_callback(message->topic, data_ptr, message->payloadlen);
}

bool Mqtt::isConnected()
{
    return m_connected;
}

void Mqtt::on_publish(int mid)
{
    // TODO: use message id to confirm send messages
    // std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}

void Mqtt::sendMessage()
{
    debugMessage("Sending message: \"{}\", on topic: \"{}\"", m_message_buffer, m_topic_buffer);
    int ret = publish(NULL, m_topic_buffer, strlen(m_message_buffer), m_message_buffer, 1, false);
    clearMessageBuffer();
    clearTopicBuffer();
}

void Mqtt::setCallback(void (*callback)(char*, uint8_t*, unsigned int))
{
    m_callback = callback;
}

char* Mqtt::getMessageBuffer()
{
    return m_message_buffer;
}

char* Mqtt::getTopicBuffer()
{
    return m_topic_buffer;
}

void Mqtt::clearMessageBuffer()
{
    for(int i = 0; i < MESSAGE_BUFFER_SIZE; i++)
    {
        m_message_buffer[i] = 0;
    }
}

void Mqtt::clearTopicBuffer()
{
    for(int i = 0; i < TOPIC_BUFFER_SIZE; i++)
    {
        m_topic_buffer[i] = 0;
    }
}

bool Mqtt::isSecure()
{
    return m_is_secure;
}

} // namespace mqtt