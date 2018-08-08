#include "EndpointColor.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

//#define ENDPOINT_COLOR_DEBUG

EndpointColor::EndpointColor(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_level(0)
  , m_state(false)
{
  m_last_send_time = millis();
  m_resend_status_time = 30;
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

char* EndpointColor::getRGBcharPtr()
{
  sprintf(m_buff, "%d;%d;%d", m_rgb.r, m_rgb.g, m_rgb.b);
  return m_buff;
}

void EndpointColor::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println("incoming message, EndpointColor");

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
    m_owner->sendMessage("sc", getRGBcharPtr(), m_id);
  }
}

void EndpointColor::sendConfig()
{
  if(m_endpoint_name != nullptr)
  {
    sprintf(m_buff, "e:color;r=%d;name=%s", m_resend_status_time, m_endpoint_name);
  }
  else
  {
    sprintf(m_buff, "e:color;r=%d;", m_resend_status_time);
  }

  m_owner->sendMessage("conf", m_buff, m_id);
}

void EndpointColor::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_COLOR_DEBUG
        Serial.println("sending status message, EndpointColor");
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
      m_owner->sendMessage("sl", m_level, m_id);
      m_owner->sendMessage("sc", getRGBcharPtr(), m_id);
    }
}

void EndpointColor::sendFeedbackMessage()
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println("sending feedback message, EndpointColor");
  #endif

  m_owner->sendMessage("sp", m_state, m_id);
  m_owner->sendMessage("sl", m_level, m_id);
  m_owner->sendMessage("sc", getRGBcharPtr(), m_id);
}