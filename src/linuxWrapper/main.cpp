#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "lib/arrrgh/arrrgh.hpp"
#include "logger.hpp"
#include <stdint.h>

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

    // USER VALUES:
    const char* device_name = "LINUX_CLIENT";
    const bool use_secure = false;
    //

    wrapperSetup();

    // HomeControlMagic hcm(device_name);

    infoMessage("info");
    errorMessage("error");
    debugMessage("debug");

    infoMessage("Exiting");

    while(true)
        ;

    return 0;
}