#include "EndpointColor.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"
#include <string>

#define ENDPOINT_COLOR_DEBUG

EndpointColor::EndpointColor(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_level(0)
  , m_state(false)
{
  m_last_send_time = millis();
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

String EndpointColor::getRGBstring()
{
  return String(m_rgb.r) + ";" + String(m_rgb.g) + ";" + String(m_rgb.b);
}

void EndpointColor::sendConfig()
{
  String buff = String("e:color;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointColor::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println(F("incoming message, EndpointColor"));

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cc"))
  {
    //TODO: extract 6 integers x;y;z;r;g;b

    m_rgb = extractRGB(payload, length);


  }

  else if(lineContains(topic, "sc"))
  {
    //m_owner->sendMessage("sc", m_color_xyz, m_id);
    m_owner->sendMessage("sc", getRGBstring(), m_id);
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
      //m_owner->sendMessage("sc", m_color_xyz, m_id);
      m_owner->sendMessage("sc", getRGBstring(), m_id);
    }
}

void EndpointColor::sendFeedback()
{
  #ifdef ENDPOINT_COLOR_DEBUG
  Serial.println(F("sending feedback message, EndpointColor"));
  #endif

  //m_owner->sendMessage("sc", m_color_xyz, m_id);
  m_owner->sendMessage("sc", getRGBstring(), m_id);
}