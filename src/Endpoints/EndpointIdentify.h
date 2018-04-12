#pragma once

#include "Endpoint.h"

class EndpointIdentify : public Endpoint
{
  public:
    EndpointIdentify(HomeControlMagic* hcm_ptr, int8_t pin);

    void sendConfig();
    void incomingMessage(char* topic, byte* payload, unsigned int length);

  private:
    uint8_t m_pin;
};
