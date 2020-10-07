#pragma once

#include "Endpoint.h"

class EndpointThermostat : public Endpoint
{
public:
    EndpointThermostat(HomeControlMagic* hcm_ptr);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);

    virtual void setTemperature(double temperature);
    virtual void setHeatingSetpoint(double temperature);

    virtual double getTemperature();
    virtual double getHeatingSetpoint();

protected:
    double m_temperature;
    double m_heating_setpoint;
};