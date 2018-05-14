#pragma once

#include "Endpoint.h"

class EndpointLevel : public Endpoint
{
  public:
    EndpointLevel(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void setLevel(uint16_t state);
    virtual uint16_t getLevel();

    virtual void sendFeedback();

  protected:
    uint16_t m_level;
    bool m_state;
    char m_buff[128];
};