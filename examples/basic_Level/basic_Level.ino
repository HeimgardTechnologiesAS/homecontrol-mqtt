#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointLevel.h"

#define DEBUG

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "DEVICE_LEVEL";

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointLevel enpointLevel(&hcm, LED_BUILTIN, false);

void setup()
{
  network.setReconnectTime(5);
  enpointLevel.setStatusTime(30);
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif
  hcm.addEndpoint(&enpointLevel);
}

void loop()
{
  hcm.doMagic();
}
