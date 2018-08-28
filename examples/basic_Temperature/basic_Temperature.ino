#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"
#include "DHT.h"
#define ESP_LOOP
#define SECURE
#define WIFI_SSID ""                        // Wifi network name
#define WIFI_PASS ""                        // Wifi password
#include "NetworkLoops.hpp"

//#define DEBUG

#define DHT_PIN 4                           // GPIO pin to use (D2).
#define DHTTYPE DHT22                       // DHT type

#define RECONNECTION_TIME 5                 // network reconnection time in seconds
#define READ_TIME 30                        // sensor reading time in seconds

static char* const GW_IP = "GW_IP";                      // gateway IP address
static char* const deviceName = "TEMPERATURE_SENSOR";    // name of device

HomeControlMagic hcm(GW_IP, deviceName, network);
EndpointTemperature endpointTemperature(&hcm);

DHT dht(DHT_PIN, DHTTYPE);

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
 #endif

  network.setReconnectTime(RECONNECTION_TIME);
  hcm.addEndpoint(&endpointTemperature);

  dht.begin();
}

void loop()
{
  static int resend_time;

  if (millis() - resend_time > READ_TIME * 1000)
  {
    resend_time = millis();

    double temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature)) {
      #ifdef DEBUG
      Serial.println("Failed to read from DHT sensor!");
      #endif
      return;
    }

    #ifdef DEBUG
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C ");
    Serial.println();
    #endif

    // this example read/set temperature every 30s and update system every 60
    // use enpointTemperature.sendFeedback() if is neccessary to update system after specific event
    endpointTemperature.setTemperature(temperature);
  }

  hcm.doMagic();
}