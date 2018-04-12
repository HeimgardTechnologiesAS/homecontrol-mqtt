#include "EndpointOnOff.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_ON_OFF_DEBUG

EndpointOnOff::EndpointOnOff(HomeControlMagic* hcm_ptr, int8_t pin, bool active_pin_state)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
  , m_active_pin_state(active_pin_state)
  , m_state(false)
{
  pinMode(m_pin, OUTPUT);
  m_last_send_time = millis();
}

void EndpointOnOff::sendConfig()
{
  String buff = String("e:on_off;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointOnOff::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_ON_OFF_DEBUG
  Serial.println("incoming message, endpoint on/off");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cp"))
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

void EndpointOnOff::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_ON_OFF_DEBUG
        Serial.println("sending status message");
      #endif
      m_owner->sendMessage("sp", m_state, m_id);
    }
}

void EndpointOnOff::controlPin()
{
  if(m_state)
  {
    digitalWrite(m_pin, m_active_pin_state);
  }
  else
  {
    digitalWrite(m_pin, !m_active_pin_state);
  }
}