#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"
#include "DHT.h"
#define ESP_LOOP
#define SECURE
#define WIFI_SSID ""                        // Wifi network name
#define WIFI_PASS ""                        // Wifi password
#include "NetworkLoops.hpp"

#define DEBUG

#define DHT_1_PIN 4                         // GPIO pin
#define DHT_2_PIN 5                         // GPIO pin
#define DHT_3_PIN 12                        // GPIO pin
#define DHT_4_PIN 14                        // GPIO pin

#define DHTTYPE DHT22                       // DHT type

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

static char* const GW_IP = "GW_IP";                      // gateway IP address
static char* const deviceName = "TEMPERATURE_SENSORS";   // name of device
static char* const username = "hc";                      // copy username from app
static char* const password = "";                        // copy password from app

HomeControlMagic hcm(GW_IP, deviceName, network, username, password);

EndpointTemperature endpointTemperature_1(&hcm);
EndpointTemperature endpointTemperature_2(&hcm);
EndpointTemperature endpointTemperature_3(&hcm);
EndpointTemperature endpointTemperature_4(&hcm);

DHT dht_1(DHT_1_PIN, DHTTYPE);
DHT dht_2(DHT_2_PIN, DHTTYPE);
DHT dht_3(DHT_3_PIN, DHTTYPE);
DHT dht_4(DHT_4_PIN, DHTTYPE);

void setup()
{
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  endpointTemperature_1.setEndpointName("NAME_1");
  endpointTemperature_2.setEndpointName("NAME_2");
  endpointTemperature_3.setEndpointName("NAME_3");
  endpointTemperature_4.setEndpointName("NAME_4");

  network.setReconnectTime(RECONNECTION_TIME);
  hcm.addEndpoint(&endpointTemperature_1);
  hcm.addEndpoint(&endpointTemperature_2);
  hcm.addEndpoint(&endpointTemperature_3);
  hcm.addEndpoint(&endpointTemperature_4);

  dht_1.begin();
  dht_2.begin();
  dht_3.begin();
  dht_4.begin();
}

void loop()
{
  static int resend_time;

  if (millis() - resend_time > 60000)
  {
    resend_time = millis();

    double temp_1 = dht_1.readTemperature();
    double temp_2 = dht_2.readTemperature();
    double temp_3 = dht_3.readTemperature();
    double temp_4 = dht_4.readTemperature();

    if (isnan(temp_1) || isnan(temp_2) || isnan(temp_3) || isnan(temp_4)) {
      #ifdef DEBUG
      Serial.println("Failed to read from DHT sensor!");
      #endif
      return;
    }

    endpointTemperature_1.setTemperature(temp_1);
    endpointTemperature_2.setTemperature(temp_2);
    endpointTemperature_3.setTemperature(temp_3);
    endpointTemperature_4.setTemperature(temp_4);
  }

  hcm.doMagic();
}
