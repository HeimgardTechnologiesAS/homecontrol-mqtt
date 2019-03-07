#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#endif

class HomeControlMagic;

class Endpoint
{
public:
    Endpoint(HomeControlMagic* hcm_ptr);

    void setStatusTime(int status_time);
    void setId(char* id);
    const char* getId();

    void setEndpointName(char* name_endpoint);
    char* getEndpointName();

    virtual void sendConfig();

    virtual void sendFeedbackMessage() = 0;

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length) = 0;

protected:
    HomeControlMagic* m_owner;
    char* m_id;
    char* m_endpoint_name = nullptr;
    char* m_config = nullptr;
};
