#include "EndpointLevel.h"
#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>

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
EndpointLevel* endpoints[max_number_of_relays];
bool outputs_state_mem[max_number_of_relays] = {true, true, true, true, true, true, true, true};
uint16_t outputs_level_mem[max_number_of_relays] = {0, 0, 0, 0, 0, 0, 0, 0}; // milli-percent
unsigned long cycle_count = 1;
const unsigned long interval_time = 1000 * 900; // 900 seconds cycle time
bool has_changed;

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
    const std::string device_name = "rPi 8-relay PWM";
    const std::string gateway_ip = "10.0.0.6";
    // read username and password from the app
    const std::string username = "hc";
    const std::string password = "password";
    const bool use_secure = true; // use mqtt with encryption

    wiringPiSetup();

    wrapperSetup();

    mqtt::Mqtt mqtt(getUniqueId(), gateway_ip, username, password, use_secure);
    linuxSetMqttPtr(&mqtt);

    HomeControlMagic hcm(device_name.c_str());

    hcm.setup();

    std::ifstream infile("config.cfg");

    for(int i = 0; i < number_of_relays; ++i)
    {
        double start_level = outputs_level_mem[i];
        bool start_state = outputs_state[i];
        if(infile.good())
        {
            infile >> start_state;
            infile >> start_level;
        }
        infoMessage("Adding {} with default {} {}", i, start_state, start_level);
        digitalWrite(outputs[i], !active_pin_state);
        pinMode(outputs[i], OUTPUT);
        endpoints[i] = new EndpointLevel(&hcm, start_state, start_level);
        hcm.addEndpoint(endpoints[i]);
    }
    infile.close();

    while(true)
    {
        hcm.doMagic();
        handlePin();
        usleep(100000);
    }

    infoMessage("Exiting");

    return 0;
}

void writeConfigFile()
{
    debugMessage("Writing config file");
    std::ofstream outfile("config.cfg.tmp");
    for(int i = 0; i < number_of_relays; ++i)
    {
        if(i != 0)
        {
            outfile << " ";
        }
        outfile << outputs_state_mem[i] << " " << outputs_level_mem[i];
    }
    outfile.flush();
    outfile.close();
    try
    {
        std::filesystem::rename("config.cfg.tmp", "config.cfg");
    }
    catch(std::filesystem::filesystem_error& e)
    {
        errorMessage("Failed to move file. {}", e.what());
    }
}

void handlePin()
{
    int current_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    int cycle_time = current_time % interval_time;

    bool send_cycle_status = false;
    if(cycle_count != (current_time / interval_time))
    {
        send_cycle_status = true;
        cycle_count = current_time / interval_time;
    }

    bool send_status = false;

    // Loop trough all endpoints and handle them
    for(int i = 0; i < number_of_relays; ++i)
    {
        send_status = false;
        bool state = endpoints[i]->getState();
        uint16_t level = endpoints[i]->getLevel();

        // Send status update when command is received
        if(state != outputs_state_mem[i] || level != outputs_level_mem[i])
        {
            outputs_state_mem[i] = state;
            outputs_level_mem[i] = level;
            send_status = true;
            has_changed = true;
        }

        // No need to calculate if it is supposed to be off
        if(state == true)
        {
            // Split the time to do rotation
            int start_time = (interval_time / number_of_relays) * i;
            int on_duration = (interval_time / 10000) * level;
            int stop_time = ((start_time + on_duration) % interval_time);
            if(start_time < stop_time)
            {
                if(cycle_time > start_time && cycle_time < stop_time)
                {
                    state = true;
                }
                else
                {
                    state = false;
                }
            }
            else
            {
                if(cycle_time > start_time || cycle_time < stop_time)
                {
                    state = true;
                }
                else
                {
                    state = false;
                }
            }
            debugMessage("Cycle {} Relay {} state {} cycle_time {} start_time {} stop_time {}",
                         cycle_count,
                         i,
                         state,
                         cycle_time,
                         start_time,
                         stop_time);
        }

        if(state != outputs_state[i])
        {
            outputs_state[i] = state;
            if(state)
            {
                debugMessage("Turning on relay");
                digitalWrite(outputs[i], active_pin_state);
            }
            else
            {
                debugMessage("Turning off relay");
                digitalWrite(outputs[i], !active_pin_state);
            }
        }

        if(send_status == true || send_cycle_status == true)
        {
            endpoints[i]->sendFeedbackMessage();
        }
    }
    if(has_changed == true && send_cycle_status == true) // avoid writing too often
    {
        has_changed = false;
        writeConfigFile();
    }
}
