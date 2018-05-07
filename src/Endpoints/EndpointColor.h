#pragma once

#include "Endpoint.h"
#include "helperFunctions.h"

class EndpointColor : public Endpoint
{
  public:
    EndpointColor(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void setLevel(uint16_t state);
    virtual uint16_t getLevel();

    virtual int getColorR();
    virtual int getColorG();
    virtual int getColorB();

    virtual String getRGBstring();

    virtual void sendFeedback();

  protected:
    struct RGB m_rgb;
    bool m_state;
    uint16_t m_level;
};