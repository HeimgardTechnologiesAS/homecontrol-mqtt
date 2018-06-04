#include "HomeControlMagic.h"
#include "Endpoints/EndpointOnOff.h"
#define ESP_LOOP
#define WIFI_SSID ""
#define WIFI_PASS ""
#include "NetworkLoops.hpp"

//#define DEBUG

#define DEVICE_PIN LED_BUILTIN                // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                   // network reconnection time in seconds
#define STATUS_TIME 30                        // system update time in seconds

char* GW_IP = "GW_IP";                        // gateway IP address
char* deviceName = "ON_OFF_DEVICE";           // name of device

bool active_pin_state = false;                // reverse pin state

bool last_state = false;

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