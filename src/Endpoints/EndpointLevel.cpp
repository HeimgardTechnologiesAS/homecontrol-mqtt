#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_LEVEL_DEBUG

EndpointLevel::EndpointLevel(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_level(0)
  , m_state(false)
{
  m_last_send_time = millis();
}

void EndpointLevel::setState(bool state)
{
  m_state = state;
  m_owner->sendMessage("sp", m_state, m_id);
}

bool EndpointLevel::getState()
{
  return m_state;
}

void EndpointLevel::setLevel(uint16_t level)
{
  m_level = level;
  m_owner->sendMessage("sl", m_level, m_id);
}

uint16_t EndpointLevel::getLevel()
{
  return m_level;
}

void EndpointLevel::sendConfig()
{
  String buff = String("e:level;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointLevel::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_LEVEL_DEBUG
  Serial.println(F("incoming message, endpoint level"));

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cl"))
  {
    m_level = extractDouble(payload, length);
  }

  else if(lineContains(topic, "sl"))
  {
    m_owner->sendMessage("sl", m_level, m_id);
  }

  else if(lineContains(topic, "cp"))
  {
    m_state = extractBool(payload, length);
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
        Serial.println(F("sending status message"));
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
      m_owner->sendMessage("sl", m_level, m_id);
    }
}

void EndpointLevel::sendFeedback()
{
  m_owner->sendMessage("sp", m_state, m_id);
  m_owner->sendMessage("sl", m_level, m_id);
}