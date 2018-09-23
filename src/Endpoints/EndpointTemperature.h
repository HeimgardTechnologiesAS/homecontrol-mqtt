#pragma once

#include "../Endpoint.h"

class EndpointTemperature : public Endpoint
{
public:
  EndpointTemperature(HomeControlMagic* hcm_ptr);

  virtual void sendStatusMessage();

  virtual void sendFeedbackMessage();

  virtual void incomingMessage(char* topic, uint8_t* payload, unsigned int length);

  virtual void setTemperature(double temperature);
  virtual double getTemperature();

protected:
  double m_temperature;
};
