#pragma once

#include "Endpoint.h"

class EndpointOnOff : public Endpoint
{
  public:
  EndpointOnOff(HomeControlMagic* hcm_ptr, int8_t pin);

  void sendConfig();
  void incomingMessage(char* topic, byte* payload, unsigned int length);
  uint8_t m_pin;  
};
