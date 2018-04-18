#include "EndpointOnOff.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_ON_OFF_DEBUG

EndpointOnOff::EndpointOnOff(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_state(false)
{
  m_last_send_time = millis();
}

void EndpointOnOff::setState(bool state)
{
  m_state = state;
  m_owner->sendMessage("sp", m_state, m_id);
}

bool EndpointOnOff::getState()
{
  return m_state;
}

void EndpointOnOff::sendConfig()
{
  String buff = String("e:on_off;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointOnOff::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_ON_OFF_DEBUG
  Serial.println(F("incoming message, EndpointOnOff"));
  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cp"))
  {
    m_state = extractBool(payload, length);
  }
  else if(lineContains(topic, "sp"))
  {
    m_owner->sendMessage("sp", m_state, m_id);
  }
}

void EndpointOnOff::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_ON_OFF_DEBUG
        Serial.println(F("sending status message, EndpointOnOff"));
      #endif

      m_owner->sendMessage("sp", m_state, m_id);
    }
}

void EndpointOnOff::sendFeedback()
{
  #ifdef ENDPOINT_ON_OFF_DEBUG
  Serial.println(F("sending feedback message, EndpointOnOff"));
  #endif

  m_owner->sendMessage("sp", m_state, m_id);
}