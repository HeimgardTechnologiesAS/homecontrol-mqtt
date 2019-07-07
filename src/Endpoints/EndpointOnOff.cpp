#include "EndpointOnOff.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "logger.hpp"
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

void EndpointOnOff::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
#ifdef ENDPOINT_ON_OFF_DEBUG
    print(F("Incoming message, EndpointOnOff"));
#endif

    if(lineContains(topic, "cp"))
    {
        m_state = extractBool(payload, length);
        try
        {
            m_incoming_callback_function(m_endpoint_name, m_state);
        }
        catch(const std::exception& e)
        {
            errorMessage("Cant call callback: {}", e.what());
        }
    }
    else if(lineContains(topic, "sp"))
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

void EndpointOnOff::setIncomingCallbackFunction(std::function<void(char*, bool)> func)
{
    m_incoming_callback_function = func;
}

void EndpointOnOff::setId(int id)
{
    m_endp_id = id;
}