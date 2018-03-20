#include "EndpointOnOff.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_ON_OFF_DEBUG

EndpointOnOff::EndpointOnOff(HomeControlMagic* hcm_ptr, int8_t pin)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
{
  pinMode(m_pin, OUTPUT);
}

void EndpointOnOff::sendConfig()
{
  m_owner->sendMessage("conf", "e:on_off", m_id);
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
    bool state = extractBool(payload, length);
    digitalWrite(m_pin, state);
    m_owner->sendMessage("sp", digitalRead(m_pin), m_id);
  }
  else if(lineContains(topic, "sp"))
  {
    m_owner->sendMessage("sp", digitalRead(m_pin), m_id);
  }
}

