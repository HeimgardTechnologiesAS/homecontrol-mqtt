#pragma once

#include "Arduino.h"

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

    virtual void sendConfig();
    virtual void sendStatusMessage();
    virtual void incomingMessage(char* topic, byte* payload, unsigned int length);

  protected:
    HomeControlMagic* m_owner;
    char m_id[4] = {0};
    long m_resend_time;
};

