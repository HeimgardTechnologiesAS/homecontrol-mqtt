#include "EndpointIdentify.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_IDENTIFY_DEBUG

EndpointIdentify::EndpointIdentify(HomeControlMagic* hcm_ptr, int8_t pin)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
{
  pinMode(m_pin, OUTPUT);
}

void EndpointIdentify::sendConfig()
{
  String buff = String("e:identify;");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointIdentify::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_IDENTIFY_DEBUG
  Serial.println("incoming message, endpoint identify");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "ci"))
  {
    bool state = extractBool(payload, length);
    digitalWrite(m_pin, state);
    digitalWrite(m_pin, !state);
  }
}