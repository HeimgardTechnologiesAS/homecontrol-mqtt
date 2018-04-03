#pragma once

#include "Endpoint.h"

class EndpointOnOff : public Endpoint
{
  public:
    EndpointOnOff(HomeControlMagic* hcm_ptr, int8_t pin);

    void sendConfig();
    void sendStatusMessage();
    void incomingMessage(char* topic, byte* payload, unsigned int length);

  private:
    uint8_t m_pin;
};
