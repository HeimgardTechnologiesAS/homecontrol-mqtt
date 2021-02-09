#pragma once

#include "Endpoint.h"

class EndpointIdentify : public Endpoint
{
public:
    EndpointIdentify(HomeControlMagic* hcm_ptr, int8_t pin);

    void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

private:
    uint8_t m_pin;
};
