# Examples
This folder holds several examples, feel free to use these as you please.

## Arduino subfolder
Holds examples that can be compiled and run using the Arduino IDE, thus these run on NodeMCU, ESP32 and others in addition to Arduino. Others have not been tested by us. Note that Arduino does not have the capacity to do encryption.

## rPi subfolder
Holds examples for how to run the software on a Raspberry Pi OS (replaces Raspian OS). As this is based on Debian the examples will also compile and run on a Ubuntu PC, although the wiringPi library that controls the GPIO pins might cause issues.
Generally the pc examples also work on the rPi.

## pc subfolder
These examples can run on Debian systems, similar to rPi above but without linking to platform specific libraries.

# How to build
The scripts are targeting Debian based system, like Raspberry Pi OS and Ubuntu.

For rPi and PC we have made a couple of tools to simplify the startup process. You need to choose your project by selecting the folder you want to use as your base.

This script will create a folder ~/hc-mqtt where everything is placed

For rPi you should be able to run any of the rPi subfolder examples as well as any of the PC examples. PC will have compile errors on the rPi code due to the wiringPi library that handles GPIO pins.

We have two options. One will set up the build system in a subfolder to the one you are currently in, and leave you with a folder that you can run "make" in to build the software. The other will additionally add systemd scripts and set it up to run at boot.

Once run, the main.cpp file will be compiled with all necessary linkages, and be called hc-mqtt.

## Basic setup
Replace the "subfolder/program" with what you want to test, i.e "pc/basic_OnOff_virtual"
```
FOLDER="subfolder/program" source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples_rpi_pc/setup.sh)
```

## With systemd scripts
Replace the "subfolder/program" with what you want to test, i.e "pc/basic_OnOff_virtual"
```
FOLDER="subfolder/program" source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples_rpi_pc/full_setup.sh)
```

## Other distributions
Have a look at the setup.sh file and the full_setup.sh file. The commands in there shows which libraries are necessary, and should be easily converted to your distribution. Most important are the necessari libraries listed in the IF sentence at the top of setup.sh