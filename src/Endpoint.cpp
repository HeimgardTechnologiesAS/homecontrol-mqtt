#include "Endpoint.h"
#include "HomeControlMagic.h"

Endpoint::Endpoint(HomeControlMagic* hcm_ptr)
    : m_owner(hcm_ptr)
{
}


void Endpoint::setId(char* id)
{
    uint8_t i = 0;
    while (*(id + i) != '\0')
    {
        *(m_id + i) = *(id + i);
        i++;
    }
}

void Endpoint::setStatusTime(int status_time)
{
    // not allowed to set report status time under 2 seconds
    if (status_time < MIN_STATUS_TIME)
    {
        m_resend_status_time = MIN_STATUS_TIME;
    }
    else
    {
        m_resend_status_time = status_time;
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
    m_owner->sendConfig(m_config, m_resend_status_time, m_endpoint_name, m_id);
}