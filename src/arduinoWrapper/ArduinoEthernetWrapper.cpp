#include "ArduinoConfig.h"
#if defined(ETHERNET) || defined(ETHERNET2)

#include "Arduino.h"
#include "ArduinoEthernetWrapper.h"
#include "ArduinoDebugLed.h"

#ifndef ETH_MAC
#error "You need to define ETH_MAC with mac address. Make sure it is unique"
#endif

//#define ETHERNET_WRAPPER_DEBUG

static Client* m_client = nullptr;
static char m_uid[20] = {'0'};
static unsigned long m_last_time_connected = 0;
static bool m_connected = false;
static byte mac[] = ETH_MAC;

void networkLoop()
{
    switch(Ethernet.maintain())
    {
        case 0:
        {
            m_connected = true;
		    m_last_time_connected = millis();
            break;
        }
        case 1:
        {
            #ifdef ETHERNET_WRAPPER_DEBUG
            Serial.print(F("\n\rDHCP: Renew failed"));
            #endif
            m_connected = false;
            DebugLedFlash(500);
            break;
        }
        case 2:
        {
            #ifdef ETHERNET_WRAPPER_DEBUG
            Serial.print(F("\n\rDHCP: Renew success"));
            #endif
            m_connected = true;
            DebugLedState(false);
            break;
        }
        case 3:
        {
            #ifdef ETHERNET_WRAPPER_DEBUG
            Serial.print(F("\n\rDHCP: Rebind fail"));
            #endif
            m_connected = false;
            DebugLedFlash(500);
            break;
        }
        case 4:
        {
            #ifdef ETHERNET_WRAPPER_DEBUG
            Serial.print(F("\n\rDHCP: Rebind success"));
            #endif
            m_connected = true;
            DebugLedState(false);
            break;
        }
        default:
        {   
            #ifdef ETHERNET_WRAPPER_DEBUG
            Serial.print(F("\n\rDHCP: Unexpected number"));
            #endif
            break;
        }
    }

    // network loop will also keep debugLed running
    DebugLedLoop();
}

void networkSetup()
{
    #ifdef ETHERNET_WRAPPER_DEBUG
    Serial.println("In setup");
    #endif
    m_client = new EthernetClient();

    // TODO: remove sprintf
    sprintf(m_uid, "%d%d%d%d%d%d", mac[0], mac[1], mac[2], 
                                      mac[3], mac[4], mac[5]);
}

void networkStart()
{
#ifdef ETHERNET_WRAPPER_DEBUG
    Serial.println("In start");
#endif

#if defined(ETH_IP) && defined(ETH_DNS) && defined(ETH_GATEWAY) && defined(ETH_SUBNET)
    Ethernet.begin(mac, ETH_IP, ETH_DNS, ETH_GATEWAY, ETH_SUBNET);
#elif defined(ETH_IP) && defined(ETH_DNS) && defined(ETH_GATEWAY)
    Ethernet.begin(mac, ETH_IP, ETH_DNS, ETH_GATEWAY);
#elif defined(ETH_IP) && defined(ETH_DNS)
    Ethernet.begin(mac, ETH_IP, ETH_DNS);
#elif defined(ETH_IP)
    Ethernet.begin(mac, ETH_IP);
#else
    Ethernet.begin(mac);
#endif

    // Allow the hardware to sort itself out
    delay(1500);
}
void networkChipRestart()
{
    // TODO: add restart
    #ifdef ETHERNET_WRAPPER_DEBUG
    Serial.println("In restart");
    #endif
}

Client& networkGetClient()
{
    return *m_client;
}

bool networkIsConnected()
{
    return m_connected;
}

void networkSetSecure(bool secure)
{
    // atmega chips are not supporting SSL so this will always be false
}

bool networkIsSecure()
{
    // atmega chips are not supporting SSL so this will always be false
    return false;
}

void setReconnectTime(unsigned long reconnect_time)
{
    // this is here to keep interface same for wifi
}

void networkSetSsid(const char* const ssid)
{
    // this is here to keep interface same for wifi
}

void networkSetPass(const char* const pass)
{
    // this is here to keep interface same for wifi
}

char* getUniqueId()
{
    return m_uid;
}


#endif // defined(ETHERNET) || defined(ETHERNET2)