#include "EndpointTemperatureTarget.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

//#define ENDPOINT_TEMPERATURE_TARGET_DEBUG

static char* const CONFIG = "temp_tar";

EndpointTemperatureTarget::EndpointTemperatureTarget(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_temperature(0)
{
  m_last_send_time = millis();
  m_resend_status_time = 60;
  m_config = CONFIG;
}

void EndpointTemperatureTarget::setTemperature(double temperature)
{
  m_temperature = temperature;
}

double EndpointTemperatureTarget::getTemperature()
{
  return m_temperature;
}

void EndpointTemperatureTarget::setTemperatureTarget(double temperature)
{
  m_temperature_target = temperature;
}

double EndpointTemperatureTarget::getTemperatureTarget()
{
  return m_temperature_target;
}

void EndpointTemperatureTarget::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
  Serial.println(F("incoming message, EndpointTemperatureTarget"));

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "ctt"))
  {
    m_temperature_target = extractDouble(payload, length);
  }

  else if(lineContains(topic, "stt"))
  {
    m_owner->sendMessage("stt", m_temperature_target, m_id);
  }

  else if(lineContains(topic, "st"))
  {
    m_owner->sendMessage("st", m_temperature, m_id);
  }
}

void EndpointTemperatureTarget::sendStatusMessage()
{
    if (millis() - m_last_send_time > m_resend_status_time * 1000)
    {
      m_last_send_time = millis();
      #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
        Serial.println(F("sending status message, EndpointTemperatureTarget"));
      #endif

      m_owner->sendMessage("st", m_temperature, m_id);
      m_owner->sendMessage("stt", m_temperature_target, m_id);
    }
}

void EndpointTemperatureTarget::sendFeedbackMessage()
{
  #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
  Serial.println(F("sending feedback message, EndpointTemperatureTarget"));
  #endif

  m_owner->sendMessage("st", m_temperature, m_id);
  m_owner->sendMessage("stt", m_temperature_target, m_id);

}