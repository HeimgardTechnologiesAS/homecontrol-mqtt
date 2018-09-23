#pragma once

#include "../Endpoint.h"

class EndpointTemperatureTarget : public Endpoint
{
  public:
    EndpointTemperatureTarget(HomeControlMagic* hcm_ptr);

    virtual void sendStatusMessage();
    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);


    virtual void setTemperature(double temperature);
    virtual void setTemperatureTarget(double temperature);

    virtual double getTemperature();
    virtual double getTemperatureTarget();

  protected:
    double m_temperature;
    double m_temperature_target;
};