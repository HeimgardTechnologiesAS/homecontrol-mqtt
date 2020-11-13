#!/bin/bash

# go to the folder

cd ~/hc-mqtt

# Install systemd script
echo " -- Copying the systemd startup file --"
sudo cp homecontrol-mqtt/examples/compile_on_rpi/hc-mqtt.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/hc-mqtt.service
sudo chown root /lib/systemd/system/hc-mqtt.service

# Set up autostart for the daemon
echo " -- Enabling startup on boot --"
sudo systemctl daemon-reload
sudo systemctl enable hc-mqtt.service
