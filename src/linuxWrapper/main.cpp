#include "HomeControlMagic.h"
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
        std::cerr << "Parsing failed" << std::endl;
    }

    if(debug_mode.value())
    {
        std::cout << "Debug mode on" << std::endl;
        logger::enableDebugLogging();
    }

    // USER VALUES:
    const char* device_name = "LINUX_CLIENT";
    //

    HomeControlMagic hcm(device_name);

    infoMessage("info");
    errorMessage("error");
    debugMessage("debug");

    std::cout << "Exiting" << std::endl;

    return 0;
}