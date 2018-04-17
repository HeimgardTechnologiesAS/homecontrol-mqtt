#pragma once

#include "Endpoint.h"

class EndpointTemperatureTarget : public Endpoint
{
  public:
    EndpointTemperatureTarget(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);


    virtual void setTemperature(double temperature);
    virtual void setTemperatureTarget(double temperature);

    virtual double getTemperature();
    virtual double getTemperatureTarget();

    virtual void sendFeedback();

  protected:
    double m_temperature;
    double m_temperature_target;
};