#pragma once

#include "Endpoint.h"

class EndpointTemperature : public Endpoint
{
public:
    EndpointTemperature(HomeControlMagic* hcm_ptr, double start_temperature = 0);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

    virtual void setTemperature(double temperature);
    virtual double getTemperature();

protected:
    double m_temperature;
};
