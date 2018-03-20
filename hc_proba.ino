#include "WifiLoop.h"
#include "HomeControlMagic.h"
#include "EndpointOnOff.h"

#define DEBUG

const String ssid = "ISKONOVAC-SEKI";
const String pass = "hrvoje22051994";
WifiLoop wifi_loop(ssid, pass);
HomeControlMagic hcm("192.168.5.30");

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
