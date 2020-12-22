#include "EndpointThermostat.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "thrmstt";

EndpointThermostat::EndpointThermostat(HomeControlMagic* hcm_ptr, double start_level)
    : Endpoint(hcm_ptr)
    , m_temperature(start_level)
{
    m_config = CONFIG;
}

void EndpointThermostat::setTemperature(double temperature)
{
    m_temperature = temperature;
}

double EndpointThermostat::getTemperature()
{
    return m_temperature;
}

void EndpointThermostat::setHeatingSetpoint(double temperature)
{
    m_heating_setpoint = temperature;
}

double EndpointThermostat::getHeatingSetpoint()
{
    return m_heating_setpoint;
}

void EndpointThermostat::incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length)
{
#ifdef ENDPOINT_THERMOSTAT_DEBUG
    print(F("Incoming message, EndpointThermostat"));
#endif

    if(lineContains(topic, "chs", length))
    {
        m_heating_setpoint = extractDouble(payload, length);
    }

    else if(lineContains(topic, "shs", length))
    {
        m_owner->sendMessage("shs", m_heating_setpoint, m_id);
    }

    else if(lineContains(topic, "st", length))
    {
        m_owner->sendMessage("st", m_temperature, m_id);
    }
}

void EndpointThermostat::sendFeedbackMessage()
{
#ifdef ENDPOINT_THERMOSTAT_DEBUG
    print(F("Sending feedback message, EndpointThermostat"));
#endif

    m_owner->sendMessage("st", m_temperature, m_id);
    m_owner->sendMessage("shs", m_heating_setpoint, m_id);
}