#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "lev";

EndpointLevel::EndpointLevel(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
    , m_level(0)
    , m_state(false)
{
    m_config = CONFIG;
}

void EndpointLevel::setState(bool state)
{
    m_state = state;
    m_owner->sendMessage("sp", m_state, m_id);
}

bool EndpointLevel::getState()
{
    return m_state;
}

void EndpointLevel::setLevel(uint16_t level)
{
    m_level = level;
    m_owner->sendMessage("sl", m_level, m_id);
}

uint16_t EndpointLevel::getLevel()
{
    return m_level;
}

void EndpointLevel::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
#ifdef ENDPOINT_LEVEL_DEBUG
    print(F("incoming message, EndpointLevel"));
#endif

    if(lineContains(topic, "cl"))
    {
        m_level = extractInteger(payload, length);
    }

    else if(lineContains(topic, "sl"))
    {
        m_owner->sendMessage("sl", m_level, m_id);
    }

    else if(lineContains(topic, "cp"))
    {
        m_state = extractBool(payload, length);
    }

    else if(lineContains(topic, "sp"))
    {
        m_owner->sendMessage("sp", m_state, m_id);
    }
}

void EndpointLevel::sendFeedbackMessage()
{
#ifdef ENDPOINT_LEVEL_DEBUG
    print(F("Sending feedback message, EndpointLevel"));
#endif

    m_owner->sendMessage("sp", m_state, m_id);
    m_owner->sendMessage("sl", m_level, m_id);
}