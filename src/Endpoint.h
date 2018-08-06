#pragma once

#include "Arduino.h"

#define MIN_STATUS_TIME 2
class HomeControlMagic;

class Endpoint
{
  public:
    Endpoint(HomeControlMagic* hcm_ptr)
    : m_owner(hcm_ptr)
    {
    }

    void setId(char* id)
    {
      uint8_t i = 0;
      while(*(id + i) != '\0')
      {
        *(m_id + i) = *(id + i);
        i++;
      }
    }

    void setStatusTime(int status_time)
    {
      // not allowed to set report status time under 2 seconds
      if(status_time < MIN_STATUS_TIME)
      {
        m_resend_status_time = MIN_STATUS_TIME;
      }
      else
      {
        m_resend_status_time = status_time;
      }
    }

    void setEndpointName(char* name_endpoint)
    {
      m_endpoint_name = name_endpoint;
    }

    char* getEndpointName()
    {
      return m_endpoint_name;
    }

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void sendFeedbackMessage();

    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

  protected:
    HomeControlMagic* m_owner;
    char m_id[4] = {0};
    long m_last_send_time;
    int m_resend_status_time;
    char* m_endpoint_name;
};

