#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointOnOff.h"

//#define DEBUG

#define DEVICE_PIN LED_BUILTIN                // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                   // network reconnection time in seconds
#define STATUS_TIME 30                        // system update time in seconds

char* ssid = "SSID";                          // wifi SSID
char* pass = "PASS";                          // wifi password
char* GW_IP = "GW_IP";                        // gateway IP address
char* deviceName = "DEVICE_ON_OFF";           // name of device

bool active_pin_state = false;                // reverse initial pin state

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
    endpointOnOff.sendFeedbackMessage();
  }
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  network.setReconnectTime(RECONNECTION_TIME);
  endpointOnOff.setStatusTime(STATUS_TIME);
  hcm.addEndpoint(&endpointOnOff);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}