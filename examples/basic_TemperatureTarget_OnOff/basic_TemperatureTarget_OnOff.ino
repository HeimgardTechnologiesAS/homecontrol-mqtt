#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperatureTarget.h"
#include "Endpoints/EndpointOnOff.h"
#include "DHT.h"

#define DEBUG

#define DHT_PIN 4                       // ESP8266 GPIO pin to use (D2).
#define DHTTYPE DHT22                   // DHT type
#define DEVICE_PIN LED_BUILTIN          // connected heater or cooler device, built in led as example

#define RECONNECTION_TIME 5             // network reconnection time in seconds
#define STATUS_TIME 60                  // system update time in seconds
#define READ_TIME 30                    // sensor reading time in seconds

const String ssid = "SSID";
const String pass = "PASS";
char* GW_IP = "GW_IP";
const String deviceName = "TERMOSTAT";

bool active_pin_state = false;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointTemperatureTarget enpointTemperatureTarget(&hcm);
EndpointOnOff enpointOnOff(&hcm);

DHT dht(DHT_PIN, DHTTYPE);

void controlPin()
{
  if(enpointOnOff.getState())
  {
    digitalWrite(DEVICE_PIN, active_pin_state);
  }
  else
  {
    digitalWrite(DEVICE_PIN, !active_pin_state);
  }
  enpointOnOff.sendFeedback();
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  network.setReconnectTime(RECONNECTION_TIME);
  enpointTemperatureTarget.setStatusTime(STATUS_TIME);
  enpointOnOff.setStatusTime(STATUS_TIME);

  double temperature = dht.readTemperature();
  enpointTemperatureTarget.setTemperatureTarget(temperature);

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  hcm.addEndpoint(&enpointTemperatureTarget);
  hcm.addEndpoint(&enpointOnOff);

  dht.begin();
}

void loop()
{
  static int read_time;

  if (millis() - read_time > READ_TIME * 1000)
  {
    read_time = millis();

    double temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    else
    {
      enpointTemperatureTarget.setTemperature(temperature);
    }

    #ifdef DEBUG
    Serial.print("Temperature from sensor: ");
    Serial.print(enpointTemperatureTarget.getTemperature());
    Serial.print(" *C ");
    Serial.println();

    Serial.print("Temperature target: ");
    Serial.print(enpointTemperatureTarget.getTemperatureTarget());
    Serial.print(" *C ");
    Serial.println();
    #endif
  }

  controlPin();
  hcm.doMagic();
}