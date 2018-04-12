#pragma once

#include "Endpoint.h"

class EndpointLevel : public Endpoint
{
  public:
    EndpointLevel(HomeControlMagic* hcm_ptr, int8_t pin, bool active_pin_state);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);
    virtual void setStatusTime(int status_time);

  protected:
    virtual void controlPin();

    uint8_t m_pin;
    uint16_t m_level;
    int m_resend_status_time;
    bool m_active_pin_state;
    bool m_state;
};