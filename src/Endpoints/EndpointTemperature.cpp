#include "EndpointTemperature.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "tmp";

EndpointTemperature::EndpointTemperature(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
    , m_temperature(0)
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

void EndpointTemperature::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
#ifdef ENDPOINT_TEMPERATURE_DEBUG
    print(F("Incoming message, EndpointTemperature"));
#endif

    if(lineContains(topic, "st"))
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
