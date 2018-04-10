#include "EndpointTemperatureTarget.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_TEMPERATURE_TARGET_DEBUG

EndpointTemperatureTarget::EndpointTemperatureTarget(HomeControlMagic* hcm_ptr, int8_t pin, bool active_state)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
  , m_active_state(active_state)
  , m_state(false)
  , m_temperature(0)
{
  pinMode(m_pin, OUTPUT);
  m_resend_time = millis();
}

void EndpointTemperatureTarget::setStatusTime(int status_time)
{
  if(status_time < 2)
  {
    m_resend_status_time = 2;
  }
  else
  {
    m_resend_status_time = status_time;
  }
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

    controlPin();

    m_owner->sendMessage("stt", m_temperature_target, m_id);
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
    if (millis() - m_resend_time > m_resend_status_time * 1000)
    {
      m_resend_time = millis();
      #ifdef ENDPOINT_TEMPERATURE_TARGET_DEBUG
        Serial.println("sending status message");
      #endif

      m_owner->sendMessage("st", m_temperature, m_id);
      m_owner->sendMessage("stt", m_temperature_target, m_id);
    }
}

void EndpointTemperatureTarget::controlPin()
{



/*
  if(m_state)
  {
    if(m_active_state)
    {
      analogWrite(m_pin, m_temperature/10);
    }
    else
    {
      analogWrite(m_pin, (10000 - m_temperature) / 10);
    }
  }
  */
}

void EndpointTemperatureTarget::setTemperature(double temperature)
{
  m_temperature = temperature;
}