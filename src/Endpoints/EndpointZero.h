#pragma once

#include "Endpoint.h"

class EndpointZero : public Endpoint
{
  public:
    EndpointZero(HomeControlMagic* hcm_ptr);

    void sendConfig();
    void sendStatusMessage();
    void sendFeedbackMessage();

    void incomingMessage(char* topic, byte* payload, unsigned int length);
};

