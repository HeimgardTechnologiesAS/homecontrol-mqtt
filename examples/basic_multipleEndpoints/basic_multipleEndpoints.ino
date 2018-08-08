#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"
#include "DHT.h"
#define ESP_LOOP
#define WIFI_SSID ""
#define WIFI_PASS ""
#include "NetworkLoops.hpp"

#define DEBUG

#define DHT_1_PIN 4                         // GPIO pin
#define DHT_2_PIN 5                         // GPIO pin
#define DHT_3_PIN 12                        // GPIO pin
#define DHT_4_PIN 14                        // GPIO pin

#define DHTTYPE DHT22                       // DHT type

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

char* GW_IP = "GW_IP";                      // gateway IP address
char* deviceName = "TEMPERATURE_SENSORS";   // name of device

HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointTemperature enpointTemperature_1(&hcm);
EndpointTemperature enpointTemperature_2(&hcm);
EndpointTemperature enpointTemperature_3(&hcm);
EndpointTemperature enpointTemperature_4(&hcm);

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

  enpointTemperature_1.setEndpointName("TempNAME_1");
  enpointTemperature_2.setEndpointName("TempNAME_2");
  enpointTemperature_3.setEndpointName("TempNAME_3");
  enpointTemperature_4.setEndpointName("TempNAME_4");

  network.setReconnectTime(RECONNECTION_TIME);
  hcm.addEndpoint(&enpointTemperature_1);
  hcm.addEndpoint(&enpointTemperature_2);
  hcm.addEndpoint(&enpointTemperature_3);
  hcm.addEndpoint(&enpointTemperature_4);

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

    enpointTemperature_1.setTemperature(temp_1);
    enpointTemperature_2.setTemperature(temp_2);
    enpointTemperature_3.setTemperature(temp_3);
    enpointTemperature_4.setTemperature(temp_4);
  }

  hcm.doMagic();
}