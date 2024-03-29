#ifdef ESP8266
#include "ArduinoEsp8266Wrapper.h"
#include "Arduino.h"
#include "ArduinoDebugLed.h"

//#define ESP_WRAPPER_DEBUG

#define RESTART_TIME 1800000L // 30 minutes

// private vars
static Client* m_client = nullptr;
static char m_uid[20] = {'0'};
static const char* m_ssid = nullptr;
static const char* m_password = nullptr;
static unsigned long m_last_time_connected = 0;
static bool m_is_secure = false;

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

// private function declaration:
static bool networkReconnect();

// implementation:
void networkLoop()
{
    if(m_client == nullptr || m_ssid == nullptr || m_password == nullptr)
    {
#ifdef ESP_WRAPPER_DEBUG
        if(m_client == nullptr)
            Serial.println(F("m_client is null!!!"));
        if(m_ssid == nullptr)
            Serial.println(F("m_ssid is null!!!"));
        if(m_password == nullptr)
            Serial.println(F("m_password is null!!!"));
#endif
        return;
    }

    unsigned long current_time = millis();
    if(WiFi.status() == WL_CONNECTED)
    {
        m_last_time_connected = current_time;
    }
    else
    {
        DebugLedFlash(500);
    }

    if(current_time - m_last_time_connected > RESTART_TIME)
    {
        networkChipRestart();
    }

    // network loop will also keep debugLed running
    DebugLedLoop();
}

void networkSetSecure(bool secure)
{
    m_is_secure = secure;
}

bool networkIsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void networkSetup()
{

    // if we are using that small module for ESP make sure we killoff debug led
    // For my esp01-s (black pcb) to be able to use serial debug.
    // LED_BUILTIN is defined as 1 which is TX on ESP01.
#ifdef ARDUINO_ESP8266_ESP01
    DebugLedPinSet(-1);
#else
    DebugLedPinSet(LED_BUILTIN);
#endif

    if(m_is_secure)
    {
        m_client = new WiFiClientSecure();
    }
    else
    {
        m_client = new WiFiClient();
    }
    // TODO: remove sprintf
    sprintf(m_uid, "%lx", ESP.getChipId());
#ifdef ESP_WRAPPER_DEBUG
    Serial.print(F("unique id: "));
    Serial.println(m_uid);
#endif

    // turn off access point
    WiFi.mode(WIFI_STA);

    gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
#ifdef ESP_WRAPPER_DEBUG
        Serial.print("Station connected, IP: ");
        Serial.println(WiFi.localIP());
#endif
        DebugLedState(false);
    });

    disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
#ifdef ESP_WRAPPER_DEBUG
        Serial.println("Station disconnected");
#endif
        DebugLedFlash(500);
    });
}

bool networkReconnect()
{
    return true;
}

void networkChipRestart()
{
    ESP.restart();
}

void networkStart()
{
    if(m_client == nullptr || m_ssid == nullptr || m_password == nullptr)
    {
#ifdef ESP_WRAPPER_DEBUG
        Serial.println(F("ESP settings not set!!!"));
#endif
        return;
    }

    WiFi.begin(m_ssid, m_password);
}

void networkSetSsid(const char* const ssid)
{
    m_ssid = ssid;
}

void networkSetPass(const char* const pass)
{
    m_password = pass;
}

bool networkIsSecure()
{
    return m_is_secure;
}

Client& networkGetClient()
{
    return *m_client;
}

char* getUniqueId()
{
    return m_uid;
}

#endif // ESP8266
