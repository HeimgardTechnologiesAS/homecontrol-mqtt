#!/bin/bash

# go to the folder
cd ~/hc-mqtt

# Install systemd script
echo " -- Copying the systemd startup file --"
cp homecontrol-mqtt/examples_rpi_pc/hc-mqtt.service .
PWD = `pwd`
sed -i 's,FOLDER,'"$PWD"',g' hc-mqtt.service
sudo cp hc-mqtt.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/hc-mqtt.service
sudo chown root /lib/systemd/system/hc-mqtt.service

# Set up autostart for the daemon
echo " -- Enabling startup on boot --"
sudo systemctl daemon-reload
sudo systemctl enable hc-mqtt.service
echo " -- To start the service now, run \"systemctl start hc-mqtt\" --"
