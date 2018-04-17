#include "EndpointTemperatureTarget.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_TEMPERATURE_TARGET_DEBUG

EndpointTemperatureTarget::EndpointTemperatureTarget(HomeControlMagic* hcm_ptr)
  : Endpoint(hcm_ptr)
  , m_temperature(0)
{
  m_last_send_time = millis();
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

void EndpointTemperatureTarget::sendConfig()
{
  String buff = String("e:temp_target;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointTemperatureTarget::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
  Serial.println("incoming message, endpoint temperature_target");

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
        Serial.println("sending status message, temperature_target");
      #endif

      m_owner->sendMessage("st", m_temperature, m_id);
      m_owner->sendMessage("stt", m_temperature_target, m_id);
    }
}

void EndpointTemperatureTarget::sendFeedback()
{
  #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
  Serial.println("sending feedback message, EndpointTemperatureTarget");
  #endif

  m_owner->sendMessage("st", m_temperature, m_id);
  m_owner->sendMessage("stt", m_temperature_target, m_id);

}