#include "WifiLoop.h"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointLevel.h"

#define DEBUG

const String ssid = "SSID";
const String pass = "PASS";
const String deviceName = "NAME";

WifiLoop wifi_loop(ssid, pass);
HomeControlMagic hcm("GW_IP", deviceName);

EndpointLevel enpointLevel(&hcm, LED_BUILTIN, false);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  wifi_loop.setReconnectTime(5);
  enpointLevel.setStatusTime(30);
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif
  hcm.addEndpoint(&enpointLevel);
}

void loop()
{
  wifi_loop.loop();
  hcm.doMagic(wifi_loop.isWifiConnected());
}