#include "mqtt.hpp"
#include "logger.hpp"

namespace mqtt
{

const std::string CERT_PATH = "/tmp/mqtt.crt";

Mqtt::Mqtt(const char* client_id,
           const std::string& gw_ip,
           const std::string& username,
           const std::string& password,
           const bool is_secure)
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
        debugMessage("Using secure port");
        port = 8883;

        fetchCertificate();

        tls_set(CERT_PATH.c_str());
        tls_opts_set(0, "tlsv1.2");
    }
    else
    {
        port = 1883;
        debugMessage("Using unsecured port");
    }
    clearTopicBuffer();
    clearMessageBuffer();

    // make sure we have a will
    will_set(("d/" + std::string(id) + "/0/online").c_str(), 8, "offline", 1, true);

    // non blocking connection to broker request
    connect_async(host, port, keepalive);
    loop_start(); // Start thread managing connection / publish / subscribe
}

Mqtt::~Mqtt()
{
    infoMessage("Called mqtt destructor");
    loop_stop();           // Kill the thread
    mosqpp::lib_cleanup(); // Mosquitto library cleanup
}

void Mqtt::on_connect(int rc)
{
    debugMessage("Connected with status: {}.", mosqpp::connack_string(rc));

    if(rc == 0)
    {
        m_connected = true;
        // subscribe to device specific
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
    debugMessage("Disconnected with status: {}", mosqpp::connack_string(rc));
}

void Mqtt::on_subscribe(int mid, int qos_count, const int* granted_qos)
{
    debugMessage("Subscription succeeded");
}

void Mqtt::on_message(const struct mosquitto_message* message)
{
    std::vector<uint8_t> msg(static_cast<uint8_t*>(message->payload), (uint8_t*)message->payload + message->payloadlen);
    if(logger.isDebugLoggingEnabled())
    {
        std::stringstream ss;
        for(char a : msg)
        {
            ss << a;
        }
        debugMessage("Received message: \"{}\", on topic: \"{}\"", ss.str(), message->topic);
    }

    if(msg.empty())
    {
        errorMessage("Got message with size 0 on topic: \"{}\"", message->topic);
        return;
    }

    std::lock_guard<std::mutex> lock(m_incoming_mutex);
    m_incoming_deque.push_back(std::make_pair(message->topic, msg));
    if(m_incoming_deque.size() > 300)
    {
        m_incoming_deque.pop_front();
    }
}

void Mqtt::triggerReadBuffer()
{
    std::lock_guard<std::mutex> lock(m_incoming_mutex);
    for(auto& element : m_incoming_deque)
    {
        if(logger.isDebugLoggingEnabled())
        {
            std::stringstream ss;
            for(char a : element.second)
            {
                ss << a;
            }
            debugMessage("Received message: \"{}\", on topic: \"{}\"", ss.str(), element.first);
        }
        m_callback(element.first.c_str(), element.second.data(), element.second.size());
    }
    m_incoming_deque.clear();
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
    publish(NULL, m_topic_buffer, strlen(m_message_buffer), m_message_buffer, 1, false);
    clearMessageBuffer();
    clearTopicBuffer();
}

void Mqtt::setCallback(void (*callback)(const char*, const uint8_t*, const unsigned int))
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

void Mqtt::fetchCertificate()
{
    // TODO: do this properly
    std::string command = fmt::format("echo | openssl s_client -connect {}:{} 2>&1 | sed -ne '/-BEGIN "
                                      "CERTIFICATE-/,/-END CERTIFICATE-/p' > {}",
                                      host,
                                      port,
                                      CERT_PATH);
    system(command.c_str());
}

} // namespace mqtt