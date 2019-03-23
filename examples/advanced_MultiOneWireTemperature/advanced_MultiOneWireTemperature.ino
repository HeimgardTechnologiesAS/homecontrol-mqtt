/**
 * This example has been tested on a NodeMCU. It has been run on Arduino
 * before, then with max 5 sensors due to RAM restrictions, so you might
 * be lucky that it still works. No guarantees.
 *
 * This code will scan the onewire network on boot, register all
 * devices and use the MAC address for indexing. This way you can
 * add and remove devices without the config getting disrupted, the
 * correct endpoint will disappear in the app
 * It polls the sensors regularly, and reports the read values.
 * Sucessfully tested with 10+ DS18B20.
 * Connect the one-wire data line to D1. Remember 4.7kOhm resistor to
 * Vcc (4-5V)
 * Demands adding libraries OneWire and DallasTemperature
 **/

#include "HomeControlMagic.h"
#include "Endpoints/EndpointTemperature.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include <DallasTemperature.h>
#include <OneWire.h>

//#define DEBUG
#define READ_TIME 30        // sensor reading time in seconds
#define MAX_DEVICES 30 // Just a random number to avoid crazyness

IPAddress gw_ip = {192, 168, 0, 55};
static char* const deviceName = "TEMP_SENSORS"; // name of device
static const char* const wifi_ssid = "";
static const char* const wifi_pass = "";
static char* const mqtt_username = "hc"; // copy username from app
static char* const mqtt_password = "";   // copy password from app

const int oneWireInput = D1;

OneWire oneWire(oneWireInput);
DallasTemperature temps(&oneWire);

HomeControlMagic hcm(deviceName);
EndpointTemperature** endpoints;

void initSensors()
{
#ifdef DEBUG
    Serial.print("Requesting temperatures...");
#endif

    temps.requestTemperatures(); // Send the command to get temperatures

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

char* addr2str(DeviceAddress deviceAddress)
{
    static char return_me[18];
    static char* hex = "0123456789ABCDEF";
    uint8_t i, j;
    for(i = 0, j = 0; i < 8; i++)
    {
        return_me[j++] = hex[deviceAddress[i] / 16];
        return_me[j++] = hex[deviceAddress[i] & 15];
    }
    return_me[j] = '\0';
    return (return_me);
}

int deviceCount = 0;
DeviceAddress* devices;

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

    temps.begin();
    deviceCount = temps.getDeviceCount();
#ifdef DEBUG
    Serial.print("Found devices: ");
    Serial.println(deviceCount);
#endif
    if(deviceCount > MAX_DEVICES)
    {
        deviceCount = MAX_DEVICES;
    }
    devices = new DeviceAddress[deviceCount];
    endpoints = new EndpointTemperature*[deviceCount];
    for(int i = 0; i < deviceCount; ++i)
    {
        temps.getAddress(devices[i], i);
        endpoints[i] = new EndpointTemperature(&hcm);
        endpoints[i]->setEndpointName(addr2str(devices[i]));     // Using MAC as name
        hcm.addEndpoint(endpoints[i], addr2str(devices[i]) + 8); // Using only the last part of the MAC
#ifdef DEBUG
        Serial.println("Added endpoint");
#endif
    }
}

unsigned long next_send = 0;
void loop()
{
    hcm.doMagic();
    if(static_cast<long>(millis() - next_send) >= 0)
    {
        next_send = millis() + (READ_TIME * 1000);
        initSensors();
        for(int i = 0; i < deviceCount; ++i)
        {
            auto temp = temps.getTempC(devices[i]);
#ifdef DEBUG
            Serial.print("Reading temp from device ");
            Serial.print(addr2str(devices[i]));
            Serial.print(" as C:");
            Serial.println(temp);
#endif
            endpoints[i]->setTemperature(temp);
        }
    }
}
