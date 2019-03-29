#include "EndpointTemperatureTarget.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "temp_tar";

EndpointTemperatureTarget::EndpointTemperatureTarget(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
    , m_temperature(0)
{
    m_config = CONFIG;
}

void EndpointTemperatureTarget::setTemperature(double temperature)
{
    m_temperature = temperature;
}

double EndpointTemperatureTarget::getTemperature()
{
    return m_temperature;
}

void EndpointTemperatureTarget::setTemperatureTarget(double temperature)
{
    m_temperature_target = temperature;
}

double EndpointTemperatureTarget::getTemperatureTarget()
{
    return m_temperature_target;
}

void EndpointTemperatureTarget::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
#ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
    print(F("Incoming message, EndpointTemperatureTarget"));
#endif

    if(lineContains(topic, "ctt"))
    {
        m_temperature_target = extractDouble(payload, length);
    }

    else if(lineContains(topic, "stt"))
    {
        m_owner->sendMessage("stt", m_temperature_target, m_id);
    }

    else if(lineContains(topic, "st"))
    {
        m_owner->sendMessage("st", m_temperature, m_id);
    }
}

void EndpointTemperatureTarget::sendFeedbackMessage()
{
#ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
    print(F("Sending feedback message, EndpointTemperatureTarget"));
#endif

    m_owner->sendMessage("st", m_temperature, m_id);
    m_owner->sendMessage("stt", m_temperature_target, m_id);
}