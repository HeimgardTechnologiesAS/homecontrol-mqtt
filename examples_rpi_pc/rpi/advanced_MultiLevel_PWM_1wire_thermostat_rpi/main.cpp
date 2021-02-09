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
#include "EndpointThermostat.h"

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
const int channels[] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, R1, R2, R3};

// DO NOT CHANGE THESE
bool active_pin_state = false;
const int number_of_relays = 11;
const int number_of_relay_endpoints = 8;
uint8_t outputs[number_of_relays] = {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, R1, R2, R3};
bool outputs_state[number_of_relays] = {false, false, false, false, false, false, false, false, false, false, false};
EndpointLevel* level_endpoints[number_of_relays];
EndpointThermostat* thermostat_endpoint = nullptr;
bool outputs_state_mem[number_of_relays] = {true, true, true, true, true, true, true, true, true, true, true};
uint16_t outputs_level_mem[number_of_relays] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // hundredths of percent
unsigned long cycle_count = 1;
const unsigned long interval_time_endpoint = 1000 * 10; // 900 seconds cycle time
const unsigned long interval_time_no_endpoint = 100;    // 0.1 seconds cycle time
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
    else
    {
    }

    // USER VALUES:
    const std::string device_name = "rPi 8-relay PWM + 1Wire + Thermostat";
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

    for(int i = 0; i < number_of_relay_endpoints; ++i)
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
        level_endpoints[i] = new EndpointLevel(&hcm, start_state, start_level);
        hcm.addEndpoint(level_endpoints[i]);
    }
    // Add thermostat
    {
        double output_set_temp_mem = 20;
        if(infile.good())
        {
            infile >> output_set_temp_mem;
        }
        infoMessage("Adding thermostat, set temp to: {}", output_set_temp_mem);
        thermostat_endpoint = new EndpointThermostat(&hcm, output_set_temp_mem);
        hcm.addEndpoint(thermostat_endpoint);
        for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
        {
            digitalWrite(outputs[i], 0);
            pinMode(outputs[i], OUTPUT);
        }
    }
    infile.close();

    // Start temp-handling thread
    std::thread temp_thread(tempReadingLoop);

    int counter = 0;
    const int sleep_duration = 900; // 0.001s
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
    infoMessage("Writing config file");
    // We always write to a temporary file and move it, to avoid half-written files
    std::ofstream outfile("/home/pi/hc-mqtt/config.cfg.tmp");
    for(int i = 0; i < number_of_relay_endpoints; ++i)
    {
        if(i != 0)
        {
            outfile << " ";
        }
        outfile << level_endpoints[i]->getState() << " " << level_endpoints[i]->getLevel();
    }
    outfile << " " << thermostat_endpoint->getHeatingSetpoint();
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
    static int loop_counter = 0;
    ++loop_counter;
    int current_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    bool send_cycle_status = false; // Set true if a new cycle has started
    if(cycle_count != (current_time / interval_time_endpoint))
    {
        send_cycle_status = true;
        cycle_count = current_time / interval_time_endpoint;
    }
    bool send_status = false;

    // Loop trough all 8 relay endpoints and handle them
    for(int i = 0; i < number_of_relay_endpoints; ++i)
    {
        bool state = false;
        uint16_t level = 0;
        send_status = false;
        state = level_endpoints[i]->getState();
        level = level_endpoints[i]->getLevel();

        int cycle_time = current_time % interval_time_endpoint;

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
            int start_time = (interval_time_endpoint / number_of_relays) * i;

            int on_duration = (interval_time_endpoint / 10000.0) * level;
            int stop_time = ((start_time + on_duration) % interval_time_endpoint);
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
            /*debugMessage("Cycle {} Relay {} state {} cycle_time {} start_time {} stop_time {}",
                         cycle_count,
                         i,
                         state,
                         cycle_time,
                         start_time,
                         stop_time);*/
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

        if((send_status == true || send_cycle_status == true))
        {
            level_endpoints[i]->sendFeedbackMessage();
        }
    }

    // Loop trough water heater endpoints and handle them
    for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
    {
        bool state = outputs_state_mem[i];
        uint16_t level = outputs_level_mem[i];

        int cycle_time = current_time % interval_time_no_endpoint;

        // No need to calculate if it is supposed to be off
        if(level <= 0.001)
        {
            state = false;
        }
        else if(state == true)
        {
            // Split the time to do rotation
            int start_time = (interval_time_no_endpoint / 3) * i % 3;
            int on_duration = (interval_time_no_endpoint / 10000.0) * level;
            int stop_time = ((start_time + on_duration) % interval_time_no_endpoint);
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
            /*debugMessage("Cycle {} Relay {} state {} cycle_time {} start_time {} stop_time {}",
                         cycle_count,
                         i,
                         state,
                         cycle_time,
                         start_time,
                         stop_time);*/
        }

        if(state != outputs_state[i])
        {
            outputs_state[i] = state;
            if(state)
            {
                debugMessage("Turning on io");
                digitalWrite(outputs[i], 1);
            }
            else
            {
                debugMessage("Turning off io");
                digitalWrite(outputs[i], 0);
            }
        }
    }

    // Every time the temp-loop has finished
    if(temp_updated && !temperatures.empty())
    {
        double min = 100, max = 0;
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
            debugMessage("Temp {} {}⁰C", key, val);

            min = std::min(min, val);
            max = std::max(max, val);
        }

        // Thermostat handling
        double avg = (max + min) / 2;
        thermostat_endpoint->setTemperature(avg);
        thermostat_endpoint->sendFeedbackMessage();
        if(avg < thermostat_endpoint->getHeatingSetpoint() - 2.5)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::min(10000, (outputs_level_mem[i] + 300));
            }
        }
        else if(avg < thermostat_endpoint->getHeatingSetpoint() - 0.5)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::min(10000, (outputs_level_mem[i] + 100));
            }
        }
        else if(avg < thermostat_endpoint->getHeatingSetpoint() - 0.2)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::min(10000, (outputs_level_mem[i] + 50));
            }
        }
        else if(avg > thermostat_endpoint->getHeatingSetpoint() + 2.5)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::max(0, (static_cast<int>(outputs_level_mem[i]) - 300));
            }
        }
        else if(avg > thermostat_endpoint->getHeatingSetpoint() + 0.50)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::max(0, (static_cast<int>(outputs_level_mem[i]) - 100));
            }
        }
        else if(avg > thermostat_endpoint->getHeatingSetpoint() + 0.2)
        {
            for(int i = number_of_relay_endpoints; i < number_of_relays; ++i)
            {
                outputs_level_mem[i] = std::max(0, (static_cast<int>(outputs_level_mem[i]) - 50));
            }
        }
        infoMessage("Sensors {} Avg {}⁰C Set {}⁰C duty cycle to {}",
                    temperatures.size(),
                    avg,
                    thermostat_endpoint->getHeatingSetpoint(),
                    (outputs_level_mem[8] / 10000.0));
    }

    double static prev_temp = -300;
    if(prev_temp != thermostat_endpoint->getHeatingSetpoint())
    {
        has_changed = true;
    }

    if(has_changed == true && send_cycle_status == true) // avoid writing too often
    {
        prev_temp = thermostat_endpoint->getHeatingSetpoint();
        has_changed = false;
        writeConfigFile();
    }
}
