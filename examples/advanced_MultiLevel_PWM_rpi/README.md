# Multiple relay control PWM

Used for long cycle time PWM, typically useful for underfloor heating. Cycle time is adjustable etc.

This code works well with raspberry Pi connected to a relay board like this:
https://smile.amazon.com/gp/product/B07JF4D814

# Running on rPi
Go the the folder on your rPi that you want to use to build and run this command:  
```
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/advanced_MultiLevel_PWM_rpi/setup.sh)
```
This will install the necessary packages, clone the git repositories needed, and copy a Makefile and a main.cpp file that you can work from. Now you only need to run 'make' to build the example.  
  
Alternately use this command to also setup the software to start on boot and make the initial build for all libaries and the executable
```
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/advanced_MultiLevel_PWM_rpi/full_setup.sh)
```
You will probably have to change the IP and password for you HomeControl gateway, but after this command finishes all the libraries will be built. That should shorten the make command significantly for building with the fixes.
