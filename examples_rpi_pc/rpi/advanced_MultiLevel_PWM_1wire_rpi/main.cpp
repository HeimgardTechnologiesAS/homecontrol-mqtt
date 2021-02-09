#include "HomeControlMagic.h"
#include "LinuxWrapper.hpp"
#include "arrrgh.hpp"
#include "logger.hpp"
#include "mqtt.hpp"
#include <wiringPi.h>

#include <csignal>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <memory>

#include <stdint.h>
#include <unistd.h>

#include "EndpointLevel.h"
#include "EndpointOnOff.h"
#include "EndpointTemperature.h"

const int CH1 = 21;
const int CH2 = 22;
const int CH3 = 23;
const int CH4 = 27;
const int CH5 = 24;
const int CH6 = 28;
const int CH7 = 29;
const int CH8 = 25;
const int R1 = 12;
const int R2 = 13;
const int R3 = 14;
const int channels[] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8};

// DO NOT CHANGE THESE
bool active_pin_state = false;
const int number_of_relays = 8;
uint8_t outputs[number_of_relays] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8};
bool outputs_state[number_of_relays] = {false, false, false, false, false, false, false, false};
EndpointLevel* endpoints[number_of_relays];
bool outputs_state_mem[number_of_relays] = {true, true, true, true, true, true, true, true};
uint16_t outputs_level_mem[number_of_relays] = {0, 0, 0, 0, 0, 0, 0, 0}; // milli-percent
unsigned long cycle_count = 1;
const unsigned long interval_time = 1000 * 10; // 900 seconds cycle time
bool has_changed;
std::map<std::string, std::shared_ptr<EndpointTemperature>> endpoints_temperature;
std::map<std::string, double> temperatures;
std::mutex temperatures_mutex;
bool temp_updated = false; // Set when a temp reading loop finishes
bool exiting = false;

// Naming the temp sensors, not necessary but shows up in the app if you have not named them there
std::map<std::string, std::string> temperature_names = {{"28-000004d00a47", "Sensor 1"}, {"28-000004eea738", "Sensor 2"}};

// it is called handle pin just to have some references to examples for arduino
void handlePin();
void signalHandler(int signum);
void tempReadingLoop();

int main(int argc, const char* argv[])
{
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGABRT, signalHandler);

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
    const std::string device_name = "rPi 8-relay PWM + 1Wire";
    const std::string gateway_ip = "10.0.1.187";
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

    std::ifstream infile("/home/pi/hc-mqtt/config.cfg");

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

    // Start temp-handling thread
    std::thread temp_thread(tempReadingLoop);

    int counter = 0;
    const int sleep_duration = 100000; // 0.1s
    while(!exiting)
    {
        hcm.doMagic();
        handlePin();
        usleep(sleep_duration);
        // Every 30 seconds
        if(++counter % 30 * 1000000 / sleep_duration == 0)
        {
            // Add temperature sensors
            debugMessage("Checking for new sensors");
            int added = 0;
            std::lock_guard<std::mutex> lck(temperatures_mutex);
            for(auto const& [key, val] : temperatures)
            {
                if(endpoints_temperature.find(key) == endpoints_temperature.end())
                {
                    // Add the endpoint
                    endpoints_temperature.emplace(key, std::make_shared<EndpointTemperature>(&hcm, val));
                    endpoints_temperature[key]->setId(key.c_str());
                    if(temperature_names.find(key) != temperature_names.end())
                    {
                        endpoints_temperature[key]->setEndpointName(temperature_names[key].c_str());
                    }
                    hcm.addEndpoint(endpoints_temperature[key].get());
                    debugMessage("Adding {} ({} - {})", key, endpoints_temperature[key]->getEndpointName(), val);
                    ++added;
                }
            }
            if(added)
            {
                hcm.sendConfigs();
            }
        }
    }
    temp_thread.join();

    infoMessage("Exiting");

    return 0;
}

void signalHandler(int signum)
{
    exiting = true;
}

void tempReadingLoop()
{
    debugMessage("Starting thread");
    const std::string base_path = "/sys/bus/w1/devices";
    int counter = 0;
    while(!exiting)
    {
        if(++counter % 100 == 0) // Loop this for 10 seconds
        {
            if(std::filesystem::is_directory(base_path))
            {
                debugMessage("Reading files");
                std::list<std::string> paths;
                // List all sensors
                for(const auto& entry : std::filesystem::directory_iterator(base_path))
                {
                    if(!exiting && std::filesystem::is_directory(entry.path()) &&
                       std::filesystem::is_regular_file(entry.path() / "temperature"))
                    {
                        // Get sensor id
                        if(entry.path().end() != entry.path().begin())
                        {
                            std::string sensor = *std::prev(entry.path().end());
                            // Read sensor values and store
                            std::ifstream infile(entry.path() / "temperature");
                            double temp;
                            infile >> temp;
                            if(infile.good())
                            {
                                temp /= 1000;
                                if(temp == 85)
                                {
                                    errorMessage("Failed to read temperature from {}", sensor);
                                    continue;
                                }
                                debugMessage("{} Temp: {}", sensor, temp);
                                std::lock_guard<std::mutex> lck(temperatures_mutex);
                                temperatures[sensor] = temp;
                            }
                        }
                    }
                }
            }
            else
            {
                infoMessage("No 1w directory exist");
            }
            temp_updated = true;
        }
        // Sleep for 0.1 seconds, then repeat
        usleep(100000);
    }
    debugMessage("Ending thread");
}

void writeConfigFile()
{
    debugMessage("Writing config file");
    // We always write to a temporary file and move it, to avoid half-written files
    std::ofstream outfile("/home/pi/hc-mqtt/config.cfg.tmp");
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
        std::filesystem::rename("/home/pi/hc-mqtt/config.cfg.tmp", "/home/pi/hc-mqtt/config.cfg");
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

    // Every time the temp-loop has finished
    if(temp_updated)
    {
        temp_updated = false;
        std::lock_guard<std::mutex> lck(temperatures_mutex);
        for(auto const& [key, val] : temperatures)
        {
            if(endpoints_temperature.find(key) == endpoints_temperature.end())
            {
                debugMessage("Did not find an endpoint for {}", key);
                continue;
            }
            endpoints_temperature[key]->setTemperature(val);
        }
    }

    if(has_changed == true && send_cycle_status == true) // avoid writing too often
    {
        has_changed = false;
        writeConfigFile();
    }
}
