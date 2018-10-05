#include "HomeControlMagic.h"
#include "Endpoints/EndpointMotion.h"
#define ESP_LOOP
#define SECURE
#define WIFI_SSID ""                        // Wifi network name
#define WIFI_PASS ""                        // Wifi password
#include "NetworkLoops.hpp"

//#define DEBUG

#define PIR_PIN 4                           // GPIO pin
#define DEVICE_PIN LED_BUILTIN              // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

static char* const GW_IP = "GW_IP";                      // gateway IP address
static char* const deviceName = "MOTION_SENSOR";         // name of device
static char* const username = "hc";                      // copy username from app
static char* const password = "";                        // copy password from app

bool active_pin_state = false;              // reverse pin state
bool last_motion = false;

HomeControlMagic hcm(GW_IP, deviceName, network, username, password);
EndpointMotion endpointMotion(&hcm);

void controlPin()
{
  bool motion = digitalRead(PIR_PIN);
  if(motion != last_motion)
  {
    last_motion = motion;
    #ifdef DEVICE_PIN
    if(motion)
    {
      digitalWrite(DEVICE_PIN, active_pin_state);
    }
    else
    {
      digitalWrite(DEVICE_PIN, !active_pin_state);
    }
    #endif
    endpointMotion.setState(motion);
  }
}

void setup()
{
  pinMode(PIR_PIN, INPUT);
  #ifdef DEVICE_PIN
  pinMode(DEVICE_PIN, OUTPUT);
  #endif

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  network.setReconnectTime(RECONNECTION_TIME);
  hcm.addEndpoint(&endpointMotion);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}
