#include "HomeControlMagic.h"
#include "Arduino.h"
#include "helperFunctions.h"

#include "Endpoints/EndpointZero.h"

#define HCM_DEBUG

static HomeControlMagic* hcm_ptr;

void callback(char* topic, byte* payload, unsigned int length)
{
  #ifdef HCM_DEBUG
  Serial.println("got in callback");
  #endif
  // check for server announce
  if(lineContains(topic, "broadcast"))
  {
    if(lineContains((char*)payload, "serverannounce"))
    {
      hcm_ptr->announce();
      return;
    }
  }

  // it is not server announce
  uint8_t start_position = lineContains(topic, "/");
  uint8_t end_position = lineContains(topic + start_position, "/") + start_position - 1;
  uint8_t diff = end_position - start_position;

  uint8_t endpoint_id = 0;
  for(int i = 0; i < diff; i++)
  {
    endpoint_id += (topic[start_position + i] - 48) * pow(10, diff - 1 - i);
  }

  Endpoint* end_ptr = hcm_ptr->getEndpoint(endpoint_id);
  if(end_ptr != NULL)
  {
    end_ptr->incomingMessage(topic, payload, length);
  }
}

HomeControlMagic::HomeControlMagic(char* server_ip, const String deviceName, LoopObject& network_object, String username, String password)
  : m_number_of_endpoints(0)
  , m_name(deviceName)
  , m_network_object(network_object)
  , m_esp_client(*(new PubSubClient(*network_object.getClientPtr())))
  , m_led_time(0)
  , m_last_loop_time(0)
  , m_username(username)
  , m_password(password)
{
  // pointer that is used from callback to set messages
  hcm_ptr = this;

  EndpointZero* epZ = new EndpointZero(hcm_ptr);
  epZ->setId("0");
  m_endpoints_pointers[m_number_of_endpoints++] = epZ;

  m_esp_client.setServer(server_ip, 1883);
  m_esp_client.setCallback(callback);

  m_id = m_network_object.getUniqueId();

  strcat(m_base_topic, "d/");
  strcat(m_base_topic, m_id.c_str());
  strcat(m_base_topic, "/");

}

void HomeControlMagic::doMagic()
{
  m_network_object.loop();

  if(m_network_object.isConnected())
  {
    mqttLoop(true);

    if(m_broker_connected != false)
    {
      sendStatus();
    }
  }
}

void HomeControlMagic::sendMessage(String topic, String message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, m_base_topic);
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  #ifdef HCM_DEBUG
  Serial.println(message);
  #endif

  m_esp_client.publish(buffer, message.c_str());
}

void HomeControlMagic::sendMessage(String topic, bool message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, m_base_topic);
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  char buffer1[4] = {0};
  sprintf(buffer1, "%d", message);

  #ifdef HCM_DEBUG
  Serial.println(buffer1);
  #endif

  m_esp_client.publish(buffer, buffer1);
}

void HomeControlMagic::sendMessage(String topic, uint16_t message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, m_base_topic);
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  char buffer1[4] = {0};
  sprintf(buffer1, "%d", message);

  #ifdef HCM_DEBUG
  Serial.println(buffer1);
  #endif

  m_esp_client.publish(buffer, buffer1);
}

void HomeControlMagic::sendMessage(String topic, double message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, m_base_topic);
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  char buffer1[4] = {0};
  sprintf(buffer1, "%.2f", message);

  #ifdef HCM_DEBUG
  Serial.println(buffer1);
  #endif

  m_esp_client.publish(buffer, buffer1);
}

/*
 *
 */
void HomeControlMagic::mqttLoop(bool reconnect)
{
  long current_time = millis();
  if(!m_esp_client.connected() && (current_time - m_last_loop_time > 100))
  {
    m_last_loop_time = current_time;
    // flash led for visual feedback
    if(current_time - m_led_time > 2000)
    {
      m_led_time = current_time;
      m_network_object.toggleLed();
    }
    if(current_time - m_last_reconnect_attempt > m_reconnect_time && reconnect)
    {
      // Attempt to reconnect
      if(reconnectMqtt())
      {
        m_last_reconnect_attempt = 0;
        m_network_object.controlLed(false);
      }
      else
      {
        // reconnectMqtt takes few seconds if it is failing so just read new time
        m_last_reconnect_attempt = millis();
        if(current_time - m_last_time_connected > 300000) //5 mins
        {
          //restart
          m_network_object.restart();
        }
      }
    }
  }
  else
  {
    m_last_time_connected = current_time;
    m_esp_client.loop();
  }
}

bool HomeControlMagic::reconnectMqtt()
{
#ifdef HCM_DEBUG
  Serial.println("Trying to reconnect to mqtt broker");
#endif
  // Attempt to connect
  if(m_esp_client.connect(m_id.c_str(), m_username.c_str(), m_password.c_str()))
  {
#ifdef HCM_DEBUG
    Serial.println("Success");
#endif
    // ... and resubscribe
    m_esp_client.setCallback(callback);
    subscribeNow();
    announce();
    m_broker_connected = true;
    return true;
  }
  else
  {
#ifdef HCM_DEBUG
    Serial.print("failed, rc=");
    Serial.println(m_esp_client.state());
#endif
    m_broker_connected = false;
    return false;
  }
}

void HomeControlMagic::announce()
{
  sendMessage("announce", m_name, "0");
}

void HomeControlMagic::subscribeNow()
{
  char buff[20] = {0};
  strcat(buff, m_id.c_str());
  strcat(buff, "/#");
  #ifdef HCM_DEBUG
  Serial.println(buff);
  #endif

  m_esp_client.subscribe(buff);
  m_esp_client.subscribe("broadcast");
}

Endpoint* HomeControlMagic::getEndpoint(uint8_t number)
{
  if(number >= m_number_of_endpoints)
  {
    return NULL;
  }

  return m_endpoints_pointers[number];
}

String HomeControlMagic::getId()
{
  return m_id;
}

uint8_t HomeControlMagic::getNumberOfEndpoints()
{
  return m_number_of_endpoints;
}

void HomeControlMagic::addEndpoint(Endpoint* endpoint_ptr)
{
  m_endpoints_pointers[m_number_of_endpoints++] = endpoint_ptr;
  char buff[4] = {0};
  sprintf(buff, "%d", m_number_of_endpoints - 1);
#ifdef HCM_DEBUG
  Serial.print("Id to set: ");
  Serial.println(buff);
#endif
  endpoint_ptr->setId(buff);
}

void HomeControlMagic::sendConfigs()
{
  for(uint8_t i = 0; i<m_number_of_endpoints; i++)
  {
      m_endpoints_pointers[i]->sendConfig();
  }
}

void HomeControlMagic::sendStatus()
{
  for(uint8_t i = 0; i<m_number_of_endpoints; i++)
  {
      m_endpoints_pointers[i]->sendStatusMessage();
  }
}

void HomeControlMagic::setReconnectTime(uint16_t seconds)
{
  m_reconnect_time = seconds * 1000;
}


