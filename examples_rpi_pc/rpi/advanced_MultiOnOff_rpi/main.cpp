#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"
#include <wiringPi.h>

#include <stdint.h>
#include <unistd.h>

#include "EndpointOnOff.h"

const int CH1 = 21;
const int CH2 = 22;
const int CH3 = 23;
const int CH4 = 27;
const int CH5 = 24;
const int CH6 = 28;
const int CH7 = 29;
const int CH8 = 25;
const int channels[] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8};

// DO NOT CHANGE THESE
bool active_pin_state = false;
const int max_number_of_relays = 8;
uint8_t outputs[max_number_of_relays] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8};
bool outputs_state[max_number_of_relays] = {false, false, false, false, false, false, false, false};
EndpointOnOff* endpoints[max_number_of_relays];

// CHANGE THIS IF NEEDED. EXPOSES ONLY THIS NUMBER OF OUTPUTS. MAX VALUE IS 11 FOR NODEMCU
const int number_of_relays = 8;

// it is called handle pin just to have some references to examples for arduino
void handlePin();

int main(int argc, const char* argv[])
{
    arrrgh::parser parser("hc_mqtt_client", "HomeControl MQTT client example");
    const auto& debug_mode = parser.add<bool>("debug", "Turn on debug mode", 'd', arrrgh::Optional, false);

    try
    {
        parser.parse(argc, argv);
    }
    catch(...)
    {
        errorMessage("Parsing failed");
    }

    if(debug_mode.value())
    {
        infoMessage("Debug mode on");
        logger.enableDebugLogging();
    }

    // USER VALUES:
    const std::string device_name = "rPi 8 relay";
    const std::string gateway_ip = "10.0.0.5";
    // read username and password from the app
    const std::string username = "hc";
    const std::string password = "3361227";
    const bool use_secure = true; // use mqtt with encryption

    wiringPiSetup();

    wrapperSetup();

    mqtt::Mqtt mqtt(getUniqueId(), gateway_ip, username, password, use_secure);
    linuxSetMqttPtr(&mqtt);

    HomeControlMagic hcm(device_name.c_str());

    hcm.setup();

    for(int i = 0; i < number_of_relays; ++i)
    {
        infoMessage("Adding {}", i);
        digitalWrite(outputs[i], !active_pin_state);
        pinMode(outputs[i], OUTPUT);
        endpoints[i] = new EndpointOnOff(&hcm);
        hcm.addEndpoint(endpoints[i]);
    }

    while(true)
    {
        hcm.doMagic();
        handlePin();
        usleep(100000);
    }

    infoMessage("Exiting");

    return 0;
}

void handlePin()
{
    for(int i = 0; i < number_of_relays; ++i)
    {
        bool state = endpoints[i]->getState();
        if(state != outputs_state[i])
        {
            debugMessage("Switching {} to {}", i, state);
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
