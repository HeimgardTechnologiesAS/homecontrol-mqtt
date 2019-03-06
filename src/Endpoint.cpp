#include "Endpoint.h"
#include "HomeControlMagic.h"

#define ENDPOINT_DEBUG

Endpoint::Endpoint(HomeControlMagic* hcm_ptr)
    : m_owner(hcm_ptr)
{
    m_id = new char[2];
    strcpy(m_id, "0");
}

void Endpoint::setId(char* id)
{
    delete m_id;
    m_id = new char[(strlen(id)+1)];
    strcpy(m_id, id);
}

void Endpoint::setEndpointName(char* name_endpoint)
{
    m_endpoint_name = new char[strlen(name_endpoint)];
    strcpy(m_endpoint_name, name_endpoint);
#ifdef ENDPOINT_DEBUG
    Serial.print("Setting endpoint name: ");
    Serial.print(name_endpoint);
    Serial.print(" ");
    Serial.print(m_endpoint_name);
    Serial.println(m_id);
#endif
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
