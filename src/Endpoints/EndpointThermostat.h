#pragma once

#include "Endpoint.h"

class EndpointThermostat : public Endpoint
{
public:
    EndpointThermostat(HomeControlMagic* hcm_ptr);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length);

    virtual void setTemperature(double temperature);
    virtual void setHeatingSetpoint(double temperature);

    virtual double getTemperature();
    virtual double getHeatingSetpoint();

protected:
    double m_temperature;
    double m_heating_setpoint;
};