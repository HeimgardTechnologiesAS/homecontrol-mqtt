#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointMotion.h"

#define DEBUG

#define PIR_PIN 4                           // GPIO pin to use (D2).
#define DEVICE_PIN LED_BUILTIN              // GPIO pin to use, built in led as example

#define RECONNECTION_TIME 5                 // network reconnection time in seconds
#define STATUS_TIME 60                      // system update time in seconds
#define READ_TIME 30                        // sensor reading time in seconds

char* ssid = "SSID";                        // wifi SSID
char* pass = "PASS";                        // wifi password
char* GW_IP = "GW_IP";                      // gateway IP address
char* deviceName = "TEMP_SENSOR";           // name of device

bool active_pin_state = false;            // reverse initial pin state

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointMotion enpointMotion(&hcm);

void controlPin()
{
  static int resend_time;

  if (millis() - resend_time > READ_TIME * 1000)
  {
    resend_time = millis();

    bool motion = digitalRead(PIR_PIN);

    if(motion)
    {
        digitalWrite(DEVICE_PIN, active_pin_state);
    }
    else
    {
        digitalWrite(DEVICE_PIN, !active_pin_state);
    }
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
  pinMode(DEVICE_PIN, OUTPUT);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}