#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_LEVEL_DEBUG

EndpointLevel::EndpointLevel(HomeControlMagic* hcm_ptr, int8_t pin, bool active_pin_state)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
  , m_level(0)
  , m_active_pin_state(active_pin_state)
  , m_state(false)
{
  pinMode(m_pin, OUTPUT);
  m_last_send_time = millis();
}

void EndpointLevel::setStatusTime(int status_time)
{
  if(status_time < 2)
  {
    m_resend_status_time = 2;
  }
  else
  {
    m_resend_status_time = status_time;
  }
}

void EndpointLevel::sendConfig()
{
  String buff = String("e:level;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointLevel::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_LEVEL_DEBUG
  Serial.println("incoming message, endpoint level");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cl"))
  {
    m_level = extractInteger(payload, length);

    controlPin();

    m_owner->sendMessage("sl", m_level, m_id);
  }

  else if(lineContains(topic, "sl"))
  {
    m_owner->sendMessage("sl", m_level, m_id);
  }

  else if(lineContains(topic, "cp"))
  {
    m_state = extractBool(payload, length);

    controlPin();

    m_owner->sendMessage("sp", m_state, m_id);
  }

  else if(lineContains(topic, "sp"))
  {
    m_owner->sendMessage("sp", m_state, m_id);
  }
}

void EndpointLevel::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_LEVEL_DEBUG
        Serial.println("sending status message");
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
      m_owner->sendMessage("sl", m_level, m_id);
    }
}

void EndpointLevel::controlPin()
{
  if(m_state)
  {
    if(m_active_pin_state)
    {
      analogWrite(m_pin, m_level/10);
    }
    else
    {
      analogWrite(m_pin, (10000 - m_level) / 10);
    }
  }
  else
  {
    digitalWrite(m_pin, !m_active_pin_state);
  }
}