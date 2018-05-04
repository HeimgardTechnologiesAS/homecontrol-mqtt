#include "ESPLoop.hpp"
#include "HomeControlMagic.h"
#include "Endpoints/EndpointColor.h"

#define DEBUG

#define R_PIN 5                             // GPIO pin to use, as example R color - (D1)
#define G_PIN 4                             // GPIO pin to use, as example G color - (D2)
#define B_PIN 0                             // GPIO pin to use, as example B color - (D3)

#define RECONNECTION_TIME 5                 // network reconnection time in seconds
#define STATUS_TIME 30                      // system update time in seconds

const String ssid = "SSID";                 // wifi SSID
const String pass = "PASS";                 // wifi password
char* GW_IP = "GW_IP";                      // gateway IP address
const String deviceName = "DEVICE_LEVEL";   // name of device

bool active_pin_state = true;               // reverse initial pin state

bool last_state = false;
uint16_t last_level = 0;
uint16_t last_color_R = 0;
uint16_t last_color_G = 0;
uint16_t last_color_B = 0;

ESPLoop network(ssid, pass);
HomeControlMagic hcm(GW_IP, deviceName, network);

EndpointColor endpointColor(&hcm);

void controlPin()
{
  // state 0/1
  bool state = endpointColor.getState();

  // level 0-10000
  uint16_t level = endpointColor.getLevel();

  // RGB color 0-10000
  uint16_t color_R = endpointColor.getColorR();
  uint16_t color_G = endpointColor.getColorG();
  uint16_t color_B = endpointColor.getColorB();

  // TODO: delete this part when fixed initial color status in the app
  if((color_R == 0) && (color_G == 0) && (color_B == 0))
  {
    color_R = 10000;
    color_G = 5680;
    color_B = 1440;
  }

  if((state != last_state) || (last_level != level) || (color_R != last_color_R) || (color_G != last_color_G) || (color_B != last_color_B))
  {
    last_state = state;
    last_level = level;
    last_color_R = color_R;
    last_color_G = color_G;
    last_color_B = color_B;

    if(state)
    {
      if(active_pin_state)
      {
        analogWrite(R_PIN, (int)((color_R / 10) * ((double)level / 10000)));
        analogWrite(G_PIN, (int)((color_G / 10) * ((double)level / 10000)));
        analogWrite(B_PIN, (int)((color_B / 10) * ((double)level / 10000)));
      }
      else
      {
        analogWrite(R_PIN, (int)((color_R / 10) * ((double)(10000 - level) / 10000)));
        analogWrite(G_PIN, (int)((color_G / 10) * ((double)(10000 - level) / 10000)));
        analogWrite(B_PIN, (int)((color_B / 10) * ((double)(10000 - level) / 10000)));
      }
    }
    else
    {
      digitalWrite(R_PIN, !active_pin_state);
      digitalWrite(G_PIN, !active_pin_state);
      digitalWrite(B_PIN, !active_pin_state);
    }
    endpointColor.sendFeedback();
  }
}

void setup()
{
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
#endif

  network.setReconnectTime(RECONNECTION_TIME);
  endpointColor.setStatusTime(STATUS_TIME);
  hcm.addEndpoint(&endpointColor);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}