#!/bin/bash

# Run the basic setup
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/setup.sh)
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/system_setup.sh)

# Enter the folder
cd ~/hc-mqtt

# Build
make
