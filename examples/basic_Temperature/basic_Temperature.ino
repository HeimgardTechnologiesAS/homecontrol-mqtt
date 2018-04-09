#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"
#include "DHT.h"

#define DEBUG
#define DHTPIN 4  // ESP8266 GPIO pin to use (D2).
#define DHTTYPE DHT22

const String ssid = "SSID";
const String pass = "PASS";
const String deviceName = "NAME";

ESPLoop network(ssid, pass);
HomeControlMagic hcm("GW_IP", deviceName, network);

EndpointTemperature enpointTemperature(&hcm, DHTPIN, false);

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  //pinMode(DHTPIN, INPUT);
  network.setReconnectTime(5);
  enpointTemperature.setStatusTime(30);
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif
  hcm.addEndpoint(&enpointTemperature);

  dht.begin();
}

void loop()
{
  hcm.doMagic();

  double t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
}
