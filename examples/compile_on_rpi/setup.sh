#!/bin/bash

# Install necessary software
if [ -f "/etc/debian_version" ]; then
    sudo apt-get install git libmosquittopp1 libmosquittopp-dev
sudo apt-get install git libmosquittopp1

# Create a folder in home (assuming user is pi)
cd ~
mkdir hc-mqtt
cd hc-mqtt

# Clone the git repo
git clone https://github.com/HomeControlAS/homecontrol-mqtt.git --depth=1 --recurse-submodules -j4 homecontrol-mqtt

# Install systemd script
sudo cp homecontrol-mqtt/examples/compile_on_rpi/hc-mqtt.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/hc-mqtt.service
sudo chown root /lib/systemd/system/hc-mqtt.service

# Copy necessary files
cp homecontrol-mqtt/examples/compile_on_rpi/Makefile .
cp homecontrol-mqtt/examples/compile_on_rpi/main.cpp .
