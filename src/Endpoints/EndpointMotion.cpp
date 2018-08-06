#include "EndpointMotion.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

//#define ENDPOINT_MOTION_DEBUG

EndpointMotion::EndpointMotion(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_state(false)
{
  m_last_send_time = millis();
  m_resend_status_time = 60;
  m_endpoint_name = "endpointMotion";
}

void EndpointMotion::setState(bool state)
{
  m_state = state;
  m_owner->sendMessage("sm", m_state, m_id);
}

bool EndpointMotion::getState()
{
  return m_state;
}

void EndpointMotion::sendConfig()
{
  sprintf(m_buff, "e:motion;r=%d;name=%c", m_resend_status_time, m_endpoint_name);
  m_owner->sendMessage("conf", m_buff, m_id);
}

void EndpointMotion::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_MOTION_DEBUG
  Serial.println(F("incoming message, EndpointMotion"));
  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "sm"))
  {
    m_owner->sendMessage("sm", m_state, m_id);
  }
}

void EndpointMotion::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_MOTION_DEBUG
        Serial.println(F("sending status message, EndpointMotion"));
      #endif

      m_owner->sendMessage("sm", m_state, m_id);
    }
}

void EndpointMotion::sendFeedbackMessage()
{
  #ifdef ENDPOINT_MOTION_DEBUG
  Serial.println(F("sending feedback message, EndpointMotion"));
  #endif

  m_owner->sendMessage("sm", m_state, m_id);
}