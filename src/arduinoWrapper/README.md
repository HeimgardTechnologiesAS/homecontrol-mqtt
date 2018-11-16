# How to select chips
## ESP
For esp comment both ETHERNET and ETHERNET2 macros in ArduinoConfig.h
ESP8266 macro will be defined once you select board from boards menu with that chip (this includes NodeMCU and so on)

## WIZ chips
Go to ArduinoConfig.h and uncomment macro that you want:
* ETHERNET for WIZ5100 
* ETHERNET2 for WIZ5500