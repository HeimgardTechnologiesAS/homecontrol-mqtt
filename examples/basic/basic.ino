#include "WifiLoop.h"
#include "HomeControlMagic.h"
#include "EndpointOnOff.h"

#define DEBUG

const String ssid = "SSID";
const String pass = "PASS";
WifiLoop wifi_loop(ssid, pass);
HomeControlMagic hcm("IP");

EndpointOnOff enpointOnOff(&hcm, LED_BUILTIN);


void setup() {
pinMode(LED_BUILTIN, OUTPUT);
wifi_loop.setReconnectTime(5000);
#ifdef DEBUG
Serial.begin(115200);
Serial.println("Started serial");
#endif
hcm.addEndpoint(&enpointOnOff);
}

void loop() {
  wifi_loop.loop();
  hcm.doMagic(wifi_loop.isWifiConnected());
}
