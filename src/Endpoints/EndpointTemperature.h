#pragma once

#include "Endpoint.h"

class EndpointTemperature : public Endpoint
{
public:
  EndpointTemperature(HomeControlMagic* hcm_ptr);

  virtual void sendConfig();
  virtual void sendStatusMessage();
  virtual void incomingMessage(char* topic, byte* payload, unsigned int length);
  virtual void setStatusTime(int status_time);

  virtual void setTemperature(double temperature);

protected:

  double m_temperature;
  int m_resend_status_time;
};