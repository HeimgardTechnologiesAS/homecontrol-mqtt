#include "EndpointTemperature.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "tmp";

EndpointTemperature::EndpointTemperature(HomeControlMagic* hcm_ptr, double start_temperature)
    : Endpoint(hcm_ptr)
    , m_temperature(start_temperature)
{
    m_config = CONFIG;
}

void EndpointTemperature::setTemperature(double temperature)
{
    m_temperature = temperature;
    m_owner->sendMessage("st", m_temperature, m_id);
}

double EndpointTemperature::getTemperature()
{
    return m_temperature;
}

void EndpointTemperature::incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length)
{
#ifdef ENDPOINT_TEMPERATURE_DEBUG
    print(F("Incoming message, EndpointTemperature"));
#endif

    if(lineContains(topic, "st", length))
    {
        m_owner->sendMessage("st", m_temperature, m_id);
    }
}

void EndpointTemperature::sendFeedbackMessage()
{
#ifdef ENDPOINT_TEMPERATURE_DEBUG
    print(F("Sending feedback message, EndpointTemperature"));
#endif

    m_owner->sendMessage("st", m_temperature, m_id);
}
