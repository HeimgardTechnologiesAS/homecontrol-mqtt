#pragma once

#include "Endpoint.h"

class EndpointTemperatureTarget : public Endpoint
{
  public:
    EndpointTemperatureTarget(HomeControlMagic* hcm_ptr);

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);
    virtual void setStatusTime(int status_time);
    virtual void setTemperature(double temperature);
    virtual void setTemperatureTarget(double temperature);

    virtual double getTemperature();
    virtual double getTemperatureTarget();

  protected:
    uint8_t m_pin;
    int m_resend_status_time;
    bool m_active_pin_state;
    bool m_state;
    double m_temperature;
    double m_temperature_target;
};