#pragma once

#include "Endpoint.h"

class EndpointLevel : public Endpoint
{
  public:
  EndpointLevel(HomeControlMagic* hcm_ptr, int8_t pin);

  void sendConfig();
  void sendStatusMessage();
  void incomingMessage(char* topic, byte* payload, unsigned int length);
  void setStatusTime(int status_time);
  uint8_t m_pin;

  private:
  int m_resend_status_time;
};