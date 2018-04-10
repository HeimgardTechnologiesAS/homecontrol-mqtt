#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperatureTarget.h"
#include "DHT.h"

#define DEBUG
#define DHTPIN 4  // ESP8266 GPIO pin to use (D2).
#define DHTTYPE DHT22
#define HEATER LED_BUILTIN

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "NAME";

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointTemperatureTarget enpointTemperatureTarget(&hcm, HEATER, false);

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  network.setReconnectTime(5);
  enpointTemperatureTarget.setStatusTime(60);

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  hcm.addEndpoint(&enpointTemperatureTarget);

  dht.begin();
}

void loop()
{
  static int resend_time;

  if (millis() - resend_time > 10000)
  {
    resend_time = millis();

    double temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    #ifdef DEBUG
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C ");
    Serial.println();
    #endif

    enpointTemperatureTarget.setTemperature(temperature);
  }

  hcm.doMagic();

}