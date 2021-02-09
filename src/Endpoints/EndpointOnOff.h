#pragma once

#include "Endpoint.h"

class EndpointOnOff : public Endpoint
{
public:
    EndpointOnOff(HomeControlMagic* hcm_ptr, bool start_state = false);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

protected:
    bool m_state;
};
