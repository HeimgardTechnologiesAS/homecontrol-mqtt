#!/bin/bash
if [ -f "/etc/debian_version" ]; then
    sudo apt-get install git libmosquittopp1 libmosquittopp-dev
fi
git clone https://github.com/HomeControlAS/homecontrol-mqtt.git --depth=1 homecontrol-mqtt
cd homecontrol-mqtt
git submodule update --init
cd ..
cp homecontrol-mqtt/wrappers/linuxWrapper/programs/compile_on_rpi/Makefile .
cp homecontrol-mqtt/wrappers/linuxWrapper/programs/compile_on_rpi/main.cpp .
