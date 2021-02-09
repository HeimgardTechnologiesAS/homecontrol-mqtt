#include "EndpointZero.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "printWrapper.h"

EndpointZero::EndpointZero(HomeControlMagic* hcm_ptr)
    : Endpoint(hcm_ptr)
{
}

void EndpointZero::sendConfig()
{
// nothing
#ifdef ENDPOINT_ZERO_DEBUG
    print("Handling endpoint zero config");
#endif
    uint16_t ep_num = (m_owner->getNumberOfEndpoints() - 1);

    m_owner->sendMessage("conf", ep_num, m_id);
}

void EndpointZero::sendFeedbackMessage()
{
    // nothing
}

void EndpointZero::incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length)
{
#ifdef ENDPOINT_ZERO_DEBUG
    print(F("incoming message, EndpointZero"));
#endif
    m_owner->sendConfigs();
}
