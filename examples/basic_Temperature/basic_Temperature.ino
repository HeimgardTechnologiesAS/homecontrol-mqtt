#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"
#include "DHT.h"

#define DEBUG
#define DHTPIN 4  // ESP8266 GPIO pin to use (D2).
#define DHTTYPE DHT22

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "TEMP_SENSOR";

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointTemperature enpointTemperature(&hcm);

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
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
  static int resend_time;

  if (millis() - resend_time > 2000)
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

    enpointTemperature.setTemperature(temperature);
  }

  hcm.doMagic();

}