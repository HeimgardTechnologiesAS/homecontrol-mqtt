#include "EndpointOnOff.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "pwr";

EndpointOnOff::EndpointOnOff(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
    , m_state(false)
{
    m_config = CONFIG;
}

void EndpointOnOff::setState(bool state)
{
    m_state = state;
    m_owner->sendMessage("sp", m_state, m_id);
}

bool EndpointOnOff::getState()
{
    return m_state;
}

void EndpointOnOff::incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length)
{
#ifdef ENDPOINT_ON_OFF_DEBUG
    print(F("Incoming message, EndpointOnOff"));
#endif

    if(lineContains(topic, "cp", length))
    {
        m_state = extractBool(payload, length);
    }
    else if(lineContains(topic, "sp", length))
    {
        m_owner->sendMessage("sp", m_state, m_id);
    }
}

void EndpointOnOff::sendFeedbackMessage()
{
#ifdef ENDPOINT_ON_OFF_DEBUG
    print(F("Sending feedback message, EndpointOnOff"));
#endif

    m_owner->sendMessage("sp", m_state, m_id);
}
