#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "Endpoints/EndpointOnOff.h"

//#define DEBUG

#define DEVICE_PIN LED_BUILTIN // GPIO pin to use, built in led as example

IPAddress gw_ip = {192, 168, 1, 10};
static const char* const deviceName = "ON_OFF_DEVICE"; // name of device
static const char* const wifi_ssid = "WIFI-SSID";
static const char* const wifi_pass = "WIFI-PASS";
static char* const username = "hc"; // copy username from app
static char* const password = "";   // copy password from app

bool active_pin_state = false; // reverse pin state
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
#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("Started serial");
#endif

    networkSetSsid(wifi_ssid);
    networkSetPass(wifi_pass);
    networkSetSecure(true); // this must be called before setServer and networkSetup
    networkSetup();
    networkStart();

    wrapperSetServer(gw_ip);
    wrapperSetUsernamePassword(mqtt_username, mqtt_password);
    wrapperSetup();

    hcm.setup();

    // DO NOT TOUCH ANYTHING BEFORE THIS LINE IN SETUP FUNCTION

    pinMode(DEVICE_PIN, OUTPUT);
    hcm.addEndpoint(&endpointOnOff);
}

void loop()
{
    controlPin();
    hcm.doMagic();
}
