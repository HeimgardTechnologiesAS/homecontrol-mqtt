#include "EndpointIdentify.h"
#include "HomeControlMagic.h"
#include "debugDefines.h"
#include "helperFunctions.h"
#include "printWrapper.h"

static char* const CONFIG = "id";

EndpointIdentify::EndpointIdentify(HomeControlMagic* hcm_ptr, int8_t pin)
    : Endpoint(hcm_ptr)
    , m_pin(pin)
{
    m_config = CONFIG;
}

void EndpointIdentify::incomingMessage(const char* topic, const uint8_t* payload, const unsigned int length)
{
#ifdef ENDPOINT_IDENTIFY_DEBUG
    print(F("incoming message, EndpointIdentify"));
#endif

    if(lineContains(topic, "ci", length))
    {
        bool state = extractBool(payload, length);
    }
}