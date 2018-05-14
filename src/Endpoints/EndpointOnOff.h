#pragma once

#include "Endpoint.h"

class EndpointOnOff : public Endpoint
{
  public:
    EndpointOnOff(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void sendFeedback();

  protected:
    bool m_state;
    char m_buff[128];
};
