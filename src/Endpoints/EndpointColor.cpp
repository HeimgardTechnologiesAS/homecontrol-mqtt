#include "EndpointColor.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

//#define ENDPOINT_COLOR_DEBUG

static char* const CONFIG = "col";

EndpointColor::EndpointColor(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_level(0)
  , m_state(false)
{
  m_last_send_time = millis();
  m_resend_status_time = 30;
  m_config = CONFIG;
}

void EndpointColor::setState(bool state)
{
  m_state = state;
  m_owner->sendMessage("sp", m_state, m_id);
}

bool EndpointColor::getState()
{
  return m_state;
}

void EndpointColor::setLevel(uint16_t level)
{
  m_level = level;
  m_owner->sendMessage("sl", m_level, m_id);
}

uint16_t EndpointColor::getLevel()
{
  return m_level;
}

int EndpointColor::getColorR()
{
  return m_rgb.r;
}

int EndpointColor::getColorG()
{
  return m_rgb.g;
}

int EndpointColor::getColorB()
{
  return m_rgb.b;
}

void EndpointColor::getRGBcharPtr(char* buffer)
{
  char buff[6];

  itoa(m_rgb.r, buff, 10);
  strcat(buffer, buff);
  strcat(buffer, ";");

  itoa(m_rgb.g, buff, 10);
  strcat(buffer, buff);
  strcat(buffer, ";");

  itoa(m_rgb.b, buff, 10);
  strcat(buffer, buff);
}

void EndpointColor::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println(F("incoming message, EndpointColor"));

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

  else if(lineContains(topic, "cc"))
  {
    m_rgb = extractRGB(payload, length);
  }

  else if(lineContains(topic, "sc"))
  {
    getRGBcharPtr(m_owner->getMessageBufferPtr());
    m_owner->sendStringMessage("sc", m_id);
  }
}

void EndpointColor::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_COLOR_DEBUG
        Serial.println(F("sending status message, EndpointColor"));
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
      m_owner->sendMessage("sl", m_level, m_id);
      getRGBcharPtr(m_owner->getMessageBufferPtr());
      m_owner->sendStringMessage("sc", m_id);
    }
}

void EndpointColor::sendFeedbackMessage()
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println(F("sending feedback message, EndpointColor"));
  #endif

  m_owner->sendMessage("sp", m_state, m_id);
  m_owner->sendMessage("sl", m_level, m_id);
  getRGBcharPtr(m_owner->getMessageBufferPtr());
  m_owner->sendStringMessage("sc", m_id);
}