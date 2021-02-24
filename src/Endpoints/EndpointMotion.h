#pragma once

#include "Endpoint.h"

class EndpointMotion : public Endpoint
{
public:
    EndpointMotion(HomeControlMagic* hcm_ptr);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

protected:
    bool m_state;
};
