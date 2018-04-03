#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointOnOff.h"

#define DEBUG

const String ssid = "SSID";
const String pass = "PASS";
const String deviceName = "NAME";

ESPLoop network(ssid, pass);
HomeControlMagic hcm("GW_IP", deviceName, network);

EndpointOnOff enpointOnOff(&hcm, LED_BUILTIN);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  network.setReconnectTime(5);
  enpointOnOff.setStatusTime(30);
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif
  hcm.addEndpoint(&enpointOnOff);
}

void loop()
{
  hcm.doMagic();
}