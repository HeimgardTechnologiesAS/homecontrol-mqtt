#!/bin/bash

# Run the basic setup
echo " -- Running the setup.sh file for base setup --"
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples_rpi_pc/setup.sh)
echo " -- Running the system_setup.sh file for setting up the service to autostart on boot --"
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples_rpi_pc/system_setup.sh)

# Enter the folder
cd ~/hc-mqtt

# Build
echo " -- Running 'make' to build the code --"
make
echo " -- Done compiling. Now run './hc-test' to test the executable. Edit the main.cpp file and run 'make' to rebuild. --"
echo " -- You probably need to edit the IP address and user/password from the Wattle app to make this work right --"
