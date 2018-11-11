#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "Endpoints/EndpointMotion.h"

//#define DEBUG

#define PIR_PIN 4              // GPIO pin
#define DEVICE_PIN LED_BUILTIN // GPIO pin to use, built in led as example

IPAddress gw_ip = {192, 168, 1, 10};
static char* const deviceName = "MOTION_SENSOR"; // name of device
static const char* const wifi_ssid = "WIFI-SSID";
static const char* const wifi_pass = "WIFI-PASS";
static char* const mqtt_username = "hc"; // copy username from app
static char* const mqtt_password = "";   // copy password from app

bool active_pin_state = false; // reverse pin state
bool last_motion = false;

HomeControlMagic hcm(deviceName);
EndpointMotion endpointMotion(&hcm);

void controlPin()
{
    bool motion = digitalRead(PIR_PIN);
    if(motion != last_motion)
    {
        last_motion = motion;
#ifdef DEVICE_PIN
        if(motion)
        {
            digitalWrite(DEVICE_PIN, active_pin_state);
        }
        else
        {
            digitalWrite(DEVICE_PIN, !active_pin_state);
        }
#endif
        endpointMotion.setState(motion);
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

    pinMode(PIR_PIN, INPUT);
#ifdef DEVICE_PIN
    pinMode(DEVICE_PIN, OUTPUT);
#endif

    hcm.addEndpoint(&endpointMotion);
}

void loop()
{
    controlPin();
    hcm.doMagic();
}
