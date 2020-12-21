#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <unistd.h>

#include "EndpointTemperature.h"

// it is called handle pin just to have some references to examples for arduino
void handlePin(EndpointTemperature&);
double readTemperature();

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
    const std::string device_name = "LINUX_CPU_TEMP";
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

    EndpointTemperature temperature(&hcm);

    hcm.setup();

    hcm.addEndpoint(&temperature);

    while(true)
    {
        hcm.doMagic();
        handlePin(temperature);
        usleep(100000);
    }

    infoMessage("Exiting");

    return 0;
}

double readTemperature()
{
    double current_temperature = 0;

    std::ifstream infile("/sys/class/thermal/thermal_zone0/temp");
    if(infile.good())
    {
        infile >> current_temperature;
    }
    else
    {
        errorMessage("Could not read the temperature from /sys/class/thermal/thermal_zone0/temp");
        exit(-1);
    }
    return current_temperature / 1000; // is read in millidegrees
}

void handlePin(EndpointTemperature& temperature)
{
    static int loop_counter = 0;
    static double old_temperature = -300;
    if(++loop_counter % 10 != 0)
    {
        return;
    }
    double current_temperature = readTemperature();

    if(abs(current_temperature - old_temperature) > 0.5 ||
       loop_counter % 3000 == 0) // Send every 300s or >0.5 degree change
    {
        infoMessage("Received new temperature: {}", current_temperature);
        old_temperature = current_temperature;
        // tell system that we received the message
        temperature.setTemperature(current_temperature);
    }
}