#define ARDUINO
//#define SECURE  // should we use SSL encryption?
#include "HomeControlMagic.h"
#include "arduinoWrapper/ArduinoWrapper.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "Endpoints/EndpointOnOff.h"


//#define DEBUG

#define DEVICE_PIN LED_BUILTIN                // GPIO pin to use, built in led as example

IPAddress gw_ip = {192, 168, 1, 2};
static const char* const deviceName = "ON_OFF_DEVICE";           // name of device
static const char* const wifi_ssid = "ssid";
static const char* const wifi_pass = "pass";
static const char* const mqtt_username = "hc";
static const char* const mqtt_password = "magic";

bool active_pin_state = false;                // reverse pin state
bool last_state = false;

HomeControlMagic hcm(deviceName);
EndpointOnOff endpointOnOff(&hcm);

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
    endpointOnOff.sendFeedbackMessage();
  }
}

void setup()
{
  pinMode(DEVICE_PIN, OUTPUT);

  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Started serial");
  #endif

  networkSetSsid(wifi_ssid);
  networkSetPass(wifi_pass);
  networkSetSecure(false); // this must be called before setServer

  wrapperSetServer(gw_ip);
  wrapperSetUsernamePassword(mqtt_username, mqtt_password);

  hcm.addEndpoint(&endpointOnOff);
}

void loop()
{
  controlPin();
  hcm.doMagic();
}