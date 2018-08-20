#include "HomeControlMagic.h"
#include "Endpoints/EndpointLevel.h"
#define ESP_LOOP
#define WIFI_SSID ""                        // Wifi network name
#define WIFI_PASS ""                        // Wifi password
#include "NetworkLoops.hpp"

//#define DEBUG

#define DEVICE_PIN LED_BUILTIN              // GPIO pin, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

char* GW_IP = "GW_IP";                      // gateway IP address
char* deviceName = "LEVEL_DEVICE";          // name of device

bool active_pin_state = false;              // reverse pin state

bool last_state = false;
uint16_t last_level = 0;

HomeControlMagic hcm(GW_IP, deviceName, network);
EndpointLevel* endpointLevel = new EndpointLevel(&hcm);

void controlPin()
{
  bool state = endpointLevel->getState();
  uint16_t level = endpointLevel->getLevel();

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
    endpointLevel->sendFeedbackMessage();
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
  hcm.addEndpoint(endpointLevel);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}