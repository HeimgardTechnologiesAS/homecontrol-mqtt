#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperatureTarget.h"
#include "Endpoints/EndpointOnOff.h"
#include "DHT.h"

#define DEBUG

#define DHT_PIN 4                       // GPIO pin to use as example (D2)
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
bool last_state = false;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointTemperatureTarget endpointTemperatureTarget(&hcm);
EndpointOnOff endpointOnOff(&hcm);

DHT dht(DHT_PIN, DHTTYPE);

void controlPin()
{
  bool state = endpointOnOff.getState();
  if(state != last_state)
  {
    last_state = state;
    if(state)
    {
      digitalWrite(DEVICE_PIN, active_pin_state);
    }
    else
    {
      digitalWrite(DEVICE_PIN, !active_pin_state);
    }
    endpointOnOff.sendFeedback();
  }
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  network.setReconnectTime(RECONNECTION_TIME);
  endpointTemperatureTarget.setStatusTime(STATUS_TIME);
  endpointOnOff.setStatusTime(STATUS_TIME);

  double temperature = dht.readTemperature();
  endpointTemperatureTarget.setTemperatureTarget(temperature);

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  hcm.addEndpoint(&endpointTemperatureTarget);
  hcm.addEndpoint(&endpointOnOff);

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
      endpointTemperatureTarget.setTemperature(temperature);
    }

    #ifdef DEBUG
    Serial.print("Temperature from sensor: ");
    Serial.print(endpointTemperatureTarget.getTemperature());
    Serial.print(" *C ");
    Serial.println();

    Serial.print("Temperature target: ");
    Serial.print(endpointTemperatureTarget.getTemperatureTarget());
    Serial.print(" *C ");
    Serial.println();
    #endif
  }

  controlPin();
  hcm.doMagic();
}