#pragma once

#include "Endpoint.h"
#include <functional>

class EndpointOnOff : public Endpoint
{
public:
    EndpointOnOff(HomeControlMagic* hcm_ptr);

    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);

    virtual void setState(bool state);
    virtual bool getState();
    void setId(int id);
    int getId();
    void setIncomingCallbackFunction(std::function<void(char*, bool)> func);

protected:
    bool m_state;
    int m_endp_id;
    std::function<void(char*, bool)> m_incoming_callback_function;
};
