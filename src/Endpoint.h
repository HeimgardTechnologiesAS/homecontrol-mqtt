#pragma once

#include "Arduino.h"

#define MIN_STATUS_TIME 2
class HomeControlMagic;

class Endpoint
{
public:
  Endpoint(HomeControlMagic* hcm_ptr);

  void setStatusTime(int status_time);
  void setId(char* id);

  void setEndpointName(char* name_endpoint);
  char* getEndpointName();

  void sendConfig();

  virtual void sendStatusMessage() = 0;
  virtual void sendFeedbackMessage() = 0;

  virtual void incomingMessage(char* topic, byte* payload, unsigned int length) = 0;

protected:
  HomeControlMagic* m_owner;
  char m_id[4] = {0};
  long m_last_send_time;
  uint8_t m_resend_status_time;
  char* m_endpoint_name = nullptr;
  char* m_config = nullptr;
};

