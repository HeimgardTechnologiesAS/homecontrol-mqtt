#include "HomeControlMagic.h"
#include "Endpoints/EndpointColor.h"
#define ESP_LOOP
#define SECURE
#define WIFI_SSID ""                        // Wifi network name
#define WIFI_PASS ""                        // Wifi password
#include "NetworkLoops.hpp"

//#define DEBUG

#define R_PIN 5                             // GPIO pin, as example R color - (D1)
#define G_PIN 4                             // GPIO pin, as example G color - (D2)
#define B_PIN 0                             // GPIO pin, as example B color - (D3)

#define RECONNECTION_TIME 5                 // network reconnection time in seconds

static char* const GW_IP = "GW_IP";                      // gateway IP address
static char* const deviceName = "COLOR_DEVICE";          // name of device

bool active_pin_state = true;               // reverse pin state

bool last_state = false;
uint16_t last_level = 0;
uint16_t last_color_R = 0;
uint16_t last_color_G = 0;
uint16_t last_color_B = 0;

HomeControlMagic hcm(GW_IP, deviceName, network, username, password); // replace username and password with ones from app
EndpointColor endpointColor(&hcm);

uint16_t adjLevel(uint16_t color_X, uint16_t level)
{
  if(active_pin_state)
  {
    // level 0-1000
    return (int)((color_X / 10.) * ((double)level / 10000));
  }
  else
  {
    // level 1000-0
    return (int)((color_X / 10.) * ((double)(10000 - level) / 10000));
  }
}

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

  if((state != last_state) || (last_level != level) || (color_R != last_color_R) || (color_G != last_color_G) || (color_B != last_color_B))
  {
    last_state = state;
    last_level = level;
    last_color_R = color_R;
    last_color_G = color_G;
    last_color_B = color_B;

    if(state)
    {
      analogWrite(R_PIN, adjLevel(color_R, level));
      analogWrite(G_PIN, adjLevel(color_G, level));
      analogWrite(B_PIN, adjLevel(color_B, level));
    }
    else
    {
      digitalWrite(R_PIN, !active_pin_state);
      digitalWrite(G_PIN, !active_pin_state);
      digitalWrite(B_PIN, !active_pin_state);
    }
    endpointColor.sendFeedbackMessage();
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
  hcm.addEndpoint(&endpointColor);
}

void loop()
{
  hcm.doMagic();
  controlPin();
}
