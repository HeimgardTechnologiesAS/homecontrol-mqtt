#include "HomeControlMagic.h"

// in Config file define ethernet options
#include "arduinoWrapper/ArduinoConfig.h"
#include "arduinoWrapper/ArduinoNetworkInterface.h"
#include "arduinoWrapper/ArduinoWrapper.h"

#include "DHT.h"
#include "Endpoints/EndpointTemperature.h"

//#define DEBUG

#define DHT_PIN 4     // GPIO pin to use (D2).
#define DHTTYPE DHT22 // DHT type

#define READ_TIME 30 // sensor reading time in seconds

IPAddress gw_ip = {192, 168, 1, 10};                  // gateway IP address
static char* const deviceName = "TEMPERATURE_SENSOR"; // name of device
static const char* const wifi_ssid = "WIFI-SSID";
static const char* const wifi_pass = "WIFI-PASS";
static char* const username = "hc"; // copy username from app
static char* const password = "";   // copy password from app

HomeControlMagic hcm(deviceName);
EndpointTemperature endpointTemperature(&hcm);

DHT dht(DHT_PIN, DHTTYPE);

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

    hcm.addEndpoint(&endpointTemperature);

    dht.begin();
}

void loop()
{
    static int resend_time;

    if(millis() - resend_time > READ_TIME * 1000)
    {
        resend_time = millis();

        double temperature = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if(isnan(temperature))
        {
#ifdef DEBUG
            Serial.println("Failed to read from DHT sensor!");
#endif
            return;
        }

#ifdef DEBUG
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" *C ");
        Serial.println();
#endif

        // this example read/set temperature every 30s and update system every 60
        // use enpointTemperature.sendFeedback() if is neccessary to update system after specific event
        endpointTemperature.setTemperature(temperature);
    }

    hcm.doMagic();
}
