#!/bin/bash

# Run the basic setup
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/setup.sh)

# go to the folder

cd ~/hc-mqtt

# Install systemd script
sudo cp homecontrol-mqtt/examples/compile_on_rpi/hc-mqtt.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/hc-mqtt.service
sudo chown root /lib/systemd/system/hc-mqtt.service

# Set up autostart for the daemon
sudo systemctl daemon-reload
sudo systemctl enable hc-mqtt.service
