#pragma once

#include "Endpoint.h"

class EndpointMotion : public Endpoint
{
  public:
    EndpointMotion(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

  protected:
    bool m_state;
    char m_buff[128];
};
