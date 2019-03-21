#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "Endpoints/EndpointOnOff.h"
#include<vector>

//#define DEBUG

IPAddress gw_ip = {192, 168, 0, 55};
static char* const deviceName = "MULTI_ON_OFF"; // name of device
static const char* const wifi_ssid = "";
static const char* const wifi_pass = "";
static char* const mqtt_username = "hc"; // copy username from app
static char* const mqtt_password = "";   // copy password from app

// DO NOT CHANGE THESE
bool active_pin_state = false;
const int max_number_of_relays = 11;
uint8_t outputs[max_number_of_relays] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10};
bool outputs_state[max_number_of_relays] = {false, false, false, false, false, false, false, false, false, false, false};
EndpointOnOff* endpoints[max_number_of_relays];

// CHANGE THIS IF NEEDED. EXPOSES ONLY THIS NUMBER OF OUTPUTS. MAX VALUE IS 11 FOR NODEMCU
const int number_of_relays = 8;

HomeControlMagic hcm(deviceName);

void controlPin()
{
    for(int i = 0; i < number_of_relays; ++i)
    {
        bool state = endpoints[i]->getState();
        if(state != outputs_state[i])
        {
            outputs_state[i] = state;
            if(state)
            {
                digitalWrite(outputs[i], active_pin_state);
            }
            else
            {
                digitalWrite(outputs[i], !active_pin_state);
            }
            endpoints[i]->sendFeedbackMessage();
        }
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

    for(int i = 0; i < number_of_relays; ++i)
    {
        pinMode(outputs[i], OUTPUT);
        endpoints[i] = new EndpointOnOff(&hcm);
        hcm.addEndpoint(endpoints[i]);
    }
}

void loop()
{
    controlPin();
    hcm.doMagic();
}
