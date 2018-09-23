#pragma once

#include "../Endpoint.h"

class EndpointLevel : public Endpoint
{
public:
    EndpointLevel(HomeControlMagic* hcm_ptr);

    virtual void sendStatusMessage();
    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void setLevel(uint16_t state);
    virtual uint16_t getLevel();

protected:
    uint16_t m_level;
    bool m_state;
};