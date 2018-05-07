#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointLevel.h"

#define DEBUG

#define DEVICE_PIN LED_BUILTIN              // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds
#define STATUS_TIME 30                      // system update time in seconds

const String ssid = "SSID";                 // wifi SSID
const String pass = "PASS";                 // wifi password
char* GW_IP = "GW_IP";                      // gateway IP address
const String deviceName = "DEVICE_LEVEL";   // name of device

bool active_pin_state = false;              // reverse initial pin state

bool last_state = false;
uint16_t last_level = 0;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointLevel endpointLevel(&hcm);

void controlPin()
{
  bool state = endpointLevel.getState();
  uint16_t level = endpointLevel.getLevel();

  if((state != last_state) || (level != last_level))
  {
    last_state = state;
    last_level = level;
    if(state)
    {
      if(active_pin_state)
      {
        analogWrite(DEVICE_PIN, level / 10);
      }
      else
      {
        analogWrite(DEVICE_PIN, (10000 - level) / 10);
      }
    }
    else
    {
      digitalWrite(DEVICE_PIN, !active_pin_state);
    }
    endpointLevel.sendFeedback();
  }
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  network.setReconnectTime(RECONNECTION_TIME);
  endpointLevel.setStatusTime(STATUS_TIME);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif

  hcm.addEndpoint(&endpointLevel);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}