#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

//#define ENDPOINT_LEVEL_DEBUG

EndpointLevel::EndpointLevel(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_level(0)
  , m_state(false)
{
  m_last_send_time = millis();
  m_resend_status_time = 30;
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
  sprintf(m_buff, "e:level;r=%d;", m_resend_status_time);
  m_owner->sendMessage("conf", m_buff, m_id);
}

void EndpointLevel::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_LEVEL_DEBUG
  Serial.println(F("incoming message, EndpointLevel"));

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cl"))
  {
    m_level = extractInteger(payload, length);
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
        Serial.println(F("sending status message, EndpointLevel"));
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
      m_owner->sendMessage("sl", m_level, m_id);
    }
}

void EndpointLevel::sendFeedbackMessage()
{
  #ifdef ENDPOINT_LEVEL_DEBUG
  Serial.println(F("sending feedback message, EndpointLevel"));
  #endif

  m_owner->sendMessage("sp", m_state, m_id);
  m_owner->sendMessage("sl", m_level, m_id);
}