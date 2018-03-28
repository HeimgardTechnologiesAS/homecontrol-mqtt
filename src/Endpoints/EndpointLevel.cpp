#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "helperFunctions.h"

#define ENDPOINT_LEVEL_DEBUG

EndpointLevel::EndpointLevel(HomeControlMagic* hcm_ptr, int8_t pin)
  : Endpoint(hcm_ptr)
  , m_pin(pin)
{
  pinMode(m_pin, OUTPUT);
  m_resend_time = millis();
}

void EndpointLevel::setStatusTime(int status_time)
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

void EndpointLevel::sendConfig()
{
  String buff = String("e:level;r=") + m_resend_status_time + String(";");
  m_owner->sendMessage("conf", buff, m_id);
}

void EndpointLevel::incomingMessage(char* topic, byte* payload, unsigned int length)
{
  #ifdef ENDPOINT_LEVEL_DEBUG
  Serial.println("incoming message, endpoint level");

  for(int i=0; i< length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if(lineContains(topic, "cl"))
  {
    uint16_t level = extractInteger(payload, length);

#ifndef ARDUINO_ESP8266_ESP01
    level /= 40;
    analogWrite(m_pin, level);
#endif

    m_owner->sendMessage("sl", (analogRead(m_pin) * 40), m_id);
  }
  else if(lineContains(topic, "sl"))
  {
    m_owner->sendMessage("sl", (analogRead(m_pin) * 40), m_id);
  }
}

void EndpointLevel::sendStatusMessage()
{
    if (millis() - m_resend_time > m_resend_status_time * 1000)
    {
      m_resend_time = millis();
      #ifdef ENDPOINT_LEVEL_DEBUG
        Serial.println("sending status message");
      #endif

      m_owner->sendMessage("sl", (analogRead(m_pin) * 40), m_id);
    }
}