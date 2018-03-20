#include "HomeControlMagic.h"
#include "WifiLoop.h"
#include "Arduino.h"
#include "PubSubClient.h"
#include "helperFunctions.h"

#include "EndpointZero.h"

#define HCM_DEBUG

WiFiClient wifiClient;
PubSubClient m_client(wifiClient);

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
    //Serial.println(topic[start_position + i]);
    endpoint_id += (topic[start_position + i] - 48) * pow(10, diff - 1 - i);
  }
  //Serial.println(endpoint_id);

  Endpoint* end_ptr = hcm_ptr->getEndpoint(endpoint_id);
  if(end_ptr != NULL)
  {
    end_ptr->incomingMessage(topic, payload, length);
  }
}

HomeControlMagic::HomeControlMagic(char* server_ip)
  : m_number_of_endpoints(0)
  , m_name("Reflektor tavan")
{
  // pointer that is used from callback to set messages
  hcm_ptr = this;
 
  EndpointZero* epZ = new EndpointZero(hcm_ptr);
  epZ->setId("0");
  m_endpoints_pointers[m_number_of_endpoints++] = epZ;

  m_client.setServer(server_ip, 1883);
  m_client.setCallback(callback);

  sprintf(m_id, "%d", ESP.getChipId());
  
}

void HomeControlMagic::doMagic(bool connection_ok)
{
  if(connection_ok)
  {
    mqttLoop(true);
  }
}

void HomeControlMagic::sendMessage(String topic, String message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, "d/");
  strcat(buffer, m_id);
  strcat(buffer, "/");
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  m_client.publish(buffer, message.c_str());  
}

void HomeControlMagic::sendMessage(String topic, uint8_t message, char* endpoint_id)
{
  char buffer[50] = {0};
  strcat(buffer, "d/");
  strcat(buffer, m_id);
  strcat(buffer, "/");
  strcat(buffer, endpoint_id);
  strcat(buffer, "/");
  strcat(buffer, topic.c_str());
  #ifdef HCM_DEBUG
  Serial.println(buffer);
  #endif

  char buffer1[4] = {0};
  sprintf(buffer1, "%d", message);

  m_client.publish(buffer, buffer1);  
}

/*
 *
 */
void HomeControlMagic::mqttLoop(bool reconnect)
{
  static long time;
  if(!m_client.connected() && (millis() - time > 100))
  {
    time = millis();
#ifndef ARDUINO_ESP8266_ESP01
    static long m_ledTime = millis();
    if(millis() - m_ledTime > 2000)
    {
      m_ledTime = millis();
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //flash led
    }
#endif
    if(millis() - m_lastReconnectAttempt > m_reconnectTime && reconnect)
    {
      // Attempt to reconnect
      if(reconnectMqtt())
      {
        m_lastReconnectAttempt = 0;
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        m_lastReconnectAttempt = millis();
        if(millis() - m_lastTimeConnected > 300000) //5 mins
        {
          //restart
          ESP.restart();
        }
      }
    }
  }
  else
  {
    m_lastTimeConnected = millis();
    m_client.loop();
  }
}

bool HomeControlMagic::reconnectMqtt()
{
#ifdef HCM_DEBUG
  Serial.println("Trying to reconnect to mqtt broker");
#endif
  // Attempt to connect
  if(m_client.connect(m_id))
  {
#ifdef HCM_DEBUG
    Serial.println("Success");
#endif
    // ... and resubscribe
    m_client.setCallback(callback);
    subscribeNow();
    announce();
    return true;
  }
  else
  {
#ifdef HCM_DEBUG
    Serial.print("failed, rc=");
    Serial.println(m_client.state());
#endif
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
  strcat(buff, m_id);
  strcat(buff, "/#");
  #ifdef HCM_DEBUG
  Serial.println(buff); 
  #endif 
  
  m_client.subscribe(buff);
  m_client.subscribe("broadcast");
}

Endpoint* HomeControlMagic::getEndpoint(uint8_t number)
{
  if(number >= m_number_of_endpoints)
  {
    return NULL;
  }
  
  return m_endpoints_pointers[number];
}

char* HomeControlMagic::getId()
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


