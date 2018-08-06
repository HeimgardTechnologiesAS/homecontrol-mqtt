#include "HomeControlMagic.h"
#include "Endpoints/EndpointMotion.h"
#define ESP_LOOP
#define WIFI_SSID ""
#define WIFI_PASS ""
#include "NetworkLoops.hpp"

//#define DEBUG

#define PIR_PIN 4                           // GPIO pin
#define DEVICE_PIN LED_BUILTIN              // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

char* GW_IP = "GW_IP";                      // gateway IP address
char* deviceName = "MOTION_SENSOR";         // name of device

bool active_pin_state = false;              // reverse pin state
bool last_motion = false;

HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointMotion enpointMotion(&hcm);

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
    enpointMotion.setState(motion);
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
  hcm.addEndpoint(&enpointMotion);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}