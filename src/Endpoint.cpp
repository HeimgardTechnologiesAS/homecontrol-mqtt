#include "Endpoint.h"
#include "HomeControlMagic.h"

//#define ENDPOINT_DEBUG

Endpoint::Endpoint(HomeControlMagic* hcm_ptr)
    : m_owner(hcm_ptr)
{
}

void Endpoint::setId(char* id)
{
    uint8_t i = 0;
    while(*(id + i) != '\0')
    {
        *(m_id + i) = *(id + i);
        i++;
    }
}

void Endpoint::setEndpointName(char* name_endpoint)
{
    m_endpoint_name = name_endpoint;
}

char* Endpoint::getEndpointName()
{
    return m_endpoint_name;
}

void Endpoint::sendConfig()
{
#ifdef ENDPOINT_DEBUG
    Serial.print("sending config for endpoint: ");
    Serial.println(m_id);
#endif
    m_owner->sendConfig(m_config, m_endpoint_name, m_id);
}