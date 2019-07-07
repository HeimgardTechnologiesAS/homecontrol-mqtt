#pragma once

#include "Endpoint.h"
#include <functional>

class EndpointLevel : public Endpoint
{
public:
    EndpointLevel(HomeControlMagic* hcm_ptr);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();

    virtual void setLevel(uint16_t state);
    virtual uint16_t getLevel();

    void setIncomingCallbackFunctionLevel(const std::function<void(char*, int)>& func);

protected:
    uint16_t m_level;
    bool m_state;

    std::function<void(char*, int)> m_incoming_callback_function_level;
};