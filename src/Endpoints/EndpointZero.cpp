#include "EndpointZero.h"
#include "HomeControlMagic.h"

#define ENDPOINT_ZERO_DEBUG

EndpointZero::EndpointZero(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
{
}

void EndpointZero::sendConfig()
{
  // nothing
}

void EndpointZero::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_ZERO_DEBUG
  Serial.println("incoming message, endpoint zero");
  
  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  uint8_t ep_num = m_owner->getNumberOfEndpoints();
  // TODO: change this
  m_owner->sendMessage("conf", "1;", m_id);

  m_owner->sendConfigs();
}

