#pragma once

#include "ArduinoConfig.h"

// guard from defining both Boards
#if defined(ESP8266) && (defined(ETHERNET) || defined(ETHERNET2))
#error "You need to choose ESP or ETHERNET"
#elif defined(ETHERNET) && defined(ETHERNET2)
#error "You can not define both ethernet and ethernet2"
#endif

// ESP8266 is automatically defined when you select board with that chip
#ifdef ESP8266
#include "ArduinoEsp8266Wrapper.h"
#elif defined(ESP32)
#include "ArduinoEsp32Wrapper.h"
#elif defined(ETHERNET) || defined(ETHERNET2)
#include "ArduinoEthernetWrapper.h"
#else
#error "You need to choose ESP8266, ESP32, ETHERNET or ETHERNET2"
#endif
