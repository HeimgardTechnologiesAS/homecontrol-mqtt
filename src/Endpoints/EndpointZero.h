#pragma once

#include "Endpoint.h"

class EndpointZero : public Endpoint
{
public:
    EndpointZero(HomeControlMagic* hcm_ptr);

    void sendConfig();
    void sendFeedbackMessage();

    void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);
};
