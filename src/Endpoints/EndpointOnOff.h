#pragma once

#include "Endpoint.h"

class EndpointOnOff : public Endpoint
{
  public:
    EndpointOnOff(HomeControlMagic* hcm_ptr, int8_t pin, bool active_pin_state);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);
    virtual void controlPin();

  protected:
    uint8_t m_pin;
    bool m_active_pin_state;
    bool m_state;
};
