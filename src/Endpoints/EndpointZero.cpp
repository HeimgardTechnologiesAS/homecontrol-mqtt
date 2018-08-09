#include "EndpointZero.h"
#include "HomeControlMagic.h"

//#define ENDPOINT_ZERO_DEBUG

EndpointZero::EndpointZero(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
{
}

void EndpointZero::sendConfig()
{
  // nothing
}
void EndpointZero::sendStatusMessage()
{
  // nothing
}
void EndpointZero::sendFeedbackMessage()
{
  // nothing
}

void EndpointZero::incomingMessage(char* topic, byte* payload, unsigned int length)
{
#ifdef ENDPOINT_ZERO_DEBUG
  Serial.println("incoming message, EndpointZero");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
#endif

  uint16_t ep_num = (m_owner->getNumberOfEndpoints() - 1);

  m_owner->sendMessage("conf", ep_num, m_id);
  m_owner->sendConfigs();
}

