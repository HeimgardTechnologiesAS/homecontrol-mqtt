#pragma once

#include "Endpoint.h"

class EndpointLevel : public Endpoint
{
public:
    EndpointLevel(HomeControlMagic* hcm_ptr, bool start_state = false, double start_level = 0);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void setLevel(uint16_t state);
    virtual uint16_t getLevel();

protected:
    uint16_t m_level;
    bool m_state;
};