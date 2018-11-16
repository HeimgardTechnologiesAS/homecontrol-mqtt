#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "DHT.h"
#include "Endpoints/EndpointOnOff.h"
#include "Endpoints/EndpointTemperatureTarget.h"

//#define DEBUG

#define DHT_PIN 4              // GPIO pin to use as example
#define DHTTYPE DHT22          // DHT type
#define DEVICE_PIN LED_BUILTIN // connected heater or cooler device, built in led as example

#define RECONNECTION_TIME 5 // network reconnection time in seconds
#define READ_TIME 30        // sensor reading time in seconds

IPAddress gw_ip = {192, 168, 1, 10};
static char* const deviceName = "TERMOSTAT"; // name of device
static const char* const wifi_ssid = "WIFI-SSID";
static const char* const wifi_pass = "WIFI-PASS";
static char* const mqtt_username = "hc"; // copy username from app
static char* const mqtt_password = "";   // copy password from app

bool active_pin_state = false; // reverse pin state
bool last_state = false;

HomeControlMagic hcm(deviceName);

EndpointTemperatureTarget endpointTemperatureTarget(&hcm);
EndpointOnOff endpointOnOff(&hcm);

DHT dht(DHT_PIN, DHTTYPE);

void controlPin()
{
    bool state = endpointOnOff.getState();
    if(state != last_state)
    {
        last_state = state;
        if(state)
        {
            digitalWrite(DEVICE_PIN, active_pin_state);
        }
        else
        {
            digitalWrite(DEVICE_PIN, !active_pin_state);
        }
        endpointOnOff.sendFeedbackMessage();
    }
}

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

    pinMode(DEVICE_PIN, OUTPUT);

    hcm.addEndpoint(&endpointTemperatureTarget);
    hcm.addEndpoint(&endpointOnOff);

    dht.begin();

    double temperature = dht.readTemperature();
    endpointTemperatureTarget.setTemperatureTarget(temperature);
}

void loop()
{
    static int read_time;

    if(millis() - read_time > READ_TIME * 1000)
    {
        read_time = millis();

        double temperature = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if(isnan(temperature))
        {
#ifdef DEBUG
            Serial.println("Failed to read from DHT sensor!");
#endif
            return;
        }
        else
        {
            endpointTemperatureTarget.setTemperature(temperature);
        }

#ifdef DEBUG
        Serial.print("Temperature from sensor: ");
        Serial.print(endpointTemperatureTarget.getTemperature());
        Serial.print(" *C ");
        Serial.println();

        Serial.print("Temperature target: ");
        Serial.print(endpointTemperatureTarget.getTemperatureTarget());
        Serial.print(" *C ");
        Serial.println();
#endif
    }

    controlPin();
    hcm.doMagic();
}
