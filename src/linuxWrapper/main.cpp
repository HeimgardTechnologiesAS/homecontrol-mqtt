#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "lib/arrrgh/arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"
#include <stdint.h>

#include "EndpointOnOff.h"

int main(int argc, const char* argv[])
{
    logger::setupLogger();

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
        logger::enableDebugLogging();
    }

    std::string test = "test";

    infoMessage("info {}", test);
    errorMessage("error {}", test);
    debugMessage("debug {}", test);

    // USER VALUES:
    const char* device_name = "LINUX_CLIENT";
    const bool use_secure = false;
    //

    wrapperSetup();

    mqtt::Mqtt mqtt(getUniqueId(), "127.0.0.1", "hc", "3522518", use_secure);
    linuxSetMqttPtr(&mqtt);

    HomeControlMagic hcm(device_name);

    EndpointOnOff on_off(&hcm);

    hcm.setup();

    hcm.addEndpoint(&on_off);

    while(true)
    {
        hcm.doMagic();
    }

    infoMessage("Exiting");

    return 0;
}