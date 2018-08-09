#include "EndpointTemperature.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_TEMPERATURE_DEBUG

EndpointTemperature::EndpointTemperature(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_temperature(0)
{
  m_last_send_time = millis();
  m_resend_status_time = 60;
}

void EndpointTemperature::setTemperature(double temperature)
{
  m_temperature = temperature;
  m_owner->sendMessage("st", m_temperature, m_id);
}

double EndpointTemperature::getTemperature()
{
  return m_temperature;
}

void EndpointTemperature::sendConfig()
{
  if(m_endpoint_name != nullptr)
  {
    sprintf(m_buff, "e:tmp;r=%d;name=%s;", m_resend_status_time, m_endpoint_name);
  }
  else
  {
    sprintf(m_buff, "e:tmp;r=%d;", m_resend_status_time);
  }

  m_owner->sendMessage("conf", m_buff, m_id);
}

void EndpointTemperature::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_TEMPERATURE_DEBUG
  Serial.println("incoming message, EndpointTemperature");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "st"))
  {
    m_owner->sendMessage("st", m_temperature, m_id);
  }
}

void EndpointTemperature::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_TEMPERATURE_DEBUG
        Serial.println("sending status message, EndpointTemperature");
      #endif

      m_owner->sendMessage("st", m_temperature, m_id);
    }
}

void EndpointTemperature::sendFeedbackMessage()
{
  #ifdef ENDPOINT_TEMPERATURE_DEBUG
  Serial.println("sending feedback message, EndpointTemperature");
  #endif

  m_owner->sendMessage("st", m_temperature, m_id);
}