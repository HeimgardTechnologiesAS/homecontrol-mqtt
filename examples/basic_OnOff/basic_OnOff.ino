#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointOnOff.h"

#define DEBUG

#define DEVICE_PIN LED_BUILTIN          // GPIO pin to use, built in led as example
#define RECONNECTION_TIME 5             // network reconnection time in seconds
#define STATUS_TIME 30                  // system update time in seconds

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "DEVICE_ON_OFF";

bool active_pin_state = false;          // initialize output pin state (false for nodeMCU to turn on LED)

bool last_state = false;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointOnOff endpointOnOff(&hcm);

void controlPin()
{
  bool state = endpointOnOff.getState();
  if(state != last_state)
  {
    last_state = state;
    if(state)
    {
      digitalWrite(DEVICE_PIN, active_pin_state);
    }
    else
    {
      digitalWrite(DEVICE_PIN, !active_pin_state);
    }
    endpointOnOff.sendFeedback();
  }
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  network.setReconnectTime(RECONNECTION_TIME);
  endpointOnOff.setStatusTime(STATUS_TIME);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif

  hcm.addEndpoint(&endpointOnOff);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}