#!/bin/bash

# Run the basic setup
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/setup.sh)

cd ~/hc-mqtt

# Set up autostart for the daemon
sudo systemctl daemon-reload
sudo systemctl enable hc-mqtt.service

# Build
make
