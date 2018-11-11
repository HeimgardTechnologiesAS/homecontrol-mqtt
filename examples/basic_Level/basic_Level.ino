#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "Endpoints/EndpointLevel.h"

//#define DEBUG

#define DEVICE_PIN LED_BUILTIN // GPIO pin, built in led as example

IPAddress gw_ip = {192, 168, 1, 10};            // gateway IP address
static char* const deviceName = "LEVEL_DEVICE"; // name of device
static const char* const wifi_ssid = "WIFI-SSID";
static const char* const wifi_pass = "WIFI-PASS";
static char* const mqtt_username = "hc"; // copy username from app
static char* const mqtt_password = "";   // copy password from app

bool active_pin_state = false; // reverse pin state

bool last_state = false;
uint16_t last_level = 0;

hrvojHomeControlMagic hcm(deviceName);

EndpointLevel endpointLevel(&hcm);

void controlPin()
{
    bool state = endpointLevel.getState();
    uint16_t level = endpointLevel.getLevel();

    if((state != last_state) || (level != last_level))
    {
        last_state = state;
        last_level = level;
        if(state)
        {
            if(active_pin_state)
            {
                analogWrite(DEVICE_PIN, level / 10);
            }
            else
            {
                analogWrite(DEVICE_PIN, (10000 - level) / 10);
            }
        }
        else
        {
            digitalWrite(DEVICE_PIN, !active_pin_state);
        }
        endpointLevel.sendFeedbackMessage();
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

    hcm.addEndpoint(&endpointLevel);
}

void loop()
{
    controlPin();
    hcm.doMagic();
}
