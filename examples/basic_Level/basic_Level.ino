#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointLevel.h"

#define DEBUG

#define DEVICE_PIN LED_BUILTIN          // ESP8266 GPIO pin to use, built in led as example
#define RECONNECTION_TIME 5             // network reconnection time in seconds
#define STATUS_TIME 30                  // system update time in seconds

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "DEVICE_LEVEL";

bool active_pin_state = false;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointLevel enpointLevel(&hcm);

void controlPin()
{
  if(enpointLevel.getState())
  {
    if(active_pin_state)
    {
      analogWrite(DEVICE_PIN, enpointLevel.getLevel() / 10);
    }
    else
    {
      analogWrite(DEVICE_PIN, (10000 - enpointLevel.getLevel()) / 10);
    }
  }
  else
  {
    digitalWrite(DEVICE_PIN, !active_pin_state);
  }
  enpointLevel.sendFeedback();
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  network.setReconnectTime(RECONNECTION_TIME);
  enpointLevel.setStatusTime(STATUS_TIME);
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif
  hcm.addEndpoint(&enpointLevel);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}