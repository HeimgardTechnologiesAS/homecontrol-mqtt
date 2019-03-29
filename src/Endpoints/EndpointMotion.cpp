#include "EndpointMotion.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "mot";

EndpointMotion::EndpointMotion(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
    , m_state(false)
{
    m_config = CONFIG;
}

void EndpointMotion::setState(bool state)
{
    m_state = state;
    m_owner->sendMessage("sm", m_state, m_id);
}

bool EndpointMotion::getState()
{
    return m_state;
}

void EndpointMotion::incomingMessage(char* topic, uint8_t* payload, unsigned int length)
{
#ifdef ENDPOINT_MOTION_DEBUG
    print(F("Incoming message, EndpointMotion"));
#endif

    if(lineContains(topic, "sm"))
    {
        m_owner->sendMessage("sm", m_state, m_id);
    }
}

void EndpointMotion::sendFeedbackMessage()
{
#ifdef ENDPOINT_MOTION_DEBUG
    print(F("Sending feedback message, EndpointMotion"));
#endif

    m_owner->sendMessage("sm", m_state, m_id);
}