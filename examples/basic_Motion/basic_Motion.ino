#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointMotion.h"

#define DEBUG

#define PIR_PIN 4                           // GPIO pin to use (D2).
//#define DEVICE_PIN 5                        // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds
#define STATUS_TIME 60                      // system update time in seconds
#define READ_TIME 5                         // sensor reading time in seconds

char* ssid = "SSID";                        // wifi SSID
char* pass = "PASS";                        // wifi password
char* GW_IP = "GW_IP";                      // gateway IP address
char* deviceName = "MOTION_SENSOR";           // name of device

bool active_pin_state = false;              // reverse initial pin state
bool last_motion = false;

ESPLoop network(ssid, pass);
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
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
 #endif

  network.setReconnectTime(RECONNECTION_TIME);
  enpointMotion.setStatusTime(STATUS_TIME);
  hcm.addEndpoint(&enpointMotion);

  pinMode(PIR_PIN, INPUT);
  #ifdef DEVICE_PIN
  pinMode(DEVICE_PIN, OUTPUT);
  #endif
}

void loop()
{
  controlPin();
  hcm.doMagic();
}