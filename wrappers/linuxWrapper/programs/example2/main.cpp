#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"

#include <stdint.h>
#include <unistd.h>

#include "EndpointLevel.h"

// it is called handle pin just to have some references to examples for arduino
void handlePin(EndpointLevel& endpointLevel);

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

    std::string test = "test";

    infoMessage("info {}", test);
    errorMessage("error {}", test);
    debugMessage("debug {}", test);

    // USER VALUES:
    const std::string device_name = "Linux Level Example";
    const std::string gateway_ip = "192.168.5.30";
    // read username and password from the app
    const std::string username = "hc";
    const std::string password = "6287911";
    const bool use_secure = true; // use mqtt with encryption
    //

    wrapperSetup();

    mqtt::Mqtt mqtt(getUniqueId(), gateway_ip, username, password, use_secure);
    linuxSetMqttPtr(&mqtt);

    HomeControlMagic hcm(device_name.c_str());

    EndpointLevel level(&hcm);

    hcm.setup();

    hcm.addEndpoint(&level);

    while(true)
    {
        hcm.doMagic();
        handlePin(level);
        usleep(100000);
    }

    infoMessage("Exiting");

    return 0;
}

void handlePin(EndpointLevel& endpointLevel)
{
    static bool old_state = false;
    static uint16_t old_level = 0;
    bool state = endpointLevel.getState();
    uint16_t level = endpointLevel.getLevel();

    if((state != old_state) || (level != old_level))
    {
        old_state = state;
        old_level = level;
        endpointLevel.sendFeedbackMessage();
    }
}