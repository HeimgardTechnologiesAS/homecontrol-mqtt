
# Heimgard Technologies
Heimgard Technologies (formerly Homecontrol) is a Norwegian energy management and home automation provider.
More information about the system and preorders can be found here:
- [Heimgard Technologies](https://heimgard.com/)

### Why this library
HGT provides an open source Arduino library for integration with your smart home. This library enables microcontrollers, with MQTT capability, to connect to the HomeControl system. This gives you, as a user, the ability to use HomeControl solution for home automation with your DIY hardware. You can use gateway and app to have control and feedback over all your devices. It runs on super-low cost hardware and can be used to integrate Your existing projects.

### MQTT
MQTT is a machine-to-machine (M2M)/"Internet of Things" open source connectivity protocol. It was designed as an extremely lightweight publish/subscribe messaging transport. It is useful for connections with remote locations where a small code footprint is required and/or network bandwidth is at a premium.

# Getting started:
## Arduino
The Arduino Software (IDE) allows you to write examples and upload them to your board. IDE can be downloaded [here](https://www.arduino.cc/en/Main/Software). If you have a reliable internet connection, you can use the [online IDE](https://create.arduino.cc/). It will allow you to save your sketches in the cloud, having them available from any device and backed up.

To download HomeControl open source Arduino library, click the DOWNLOAD button in the top right corner and uncompress the folder. Place the homecontrol library folder to your /libraries/folder. You may need to create the libraries subfolder if this is your first library. Restart the IDE.

You can open one of the HomeControl example sketch: File > Examples > homecontrol-mqtt

Select the board You are using (Tools > Board) and correct serial port of the board (Tools > Port).
If You are using one of the verified ESP controllers, start IDE and open Preferences window. Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas. Open Boards Manager from Tools > Board menu and install esp8266 platform (don't forget to select your ESP8266 board from Tools > Board menu after installation).

Make sure you select 2.4.1/2.4.2 version when installing esp8266 platform. While 2.5.2 version is available, it has a bug that, once fixed, will allow you to compile and upload sketches on the board, but the app will not recognize it.

We also support ESP32. Tested using version 1.0.4.

On Windows, port this should be something like COM2 (or some other number), on the Mac, this should be something like /dev/cu.usbserial-1B1.

Push the reset button on the board then click the Upload button in the IDE. Wait a few seconds. If successful, the message "Done uploading." will appear in the status bar.

## Other platforms
Raspberry Pi and PC builds has more information in the subfolder [examples_rpi_pc](examples_rpi_pc)

# Examples:
## Arduino
HomeControl is providing You examples composed of specific endpoints for each application:
- basic_OnOff - with this example, it is possible to control and have a feedback with any existing device which use a digital control signal (on/off).
- basic_OnOff_ethernet - this example allows you to control any device that uses digital control signal (on/off) through Ethernet connection.
- basic_Level - example Level allows you to manage and feedback any existing device which use analog control signal. This example also uses the functionality of the basic_OnOff example.
- basic_Color - allows you to turn ON or OFF, and set color and brightness of RGB LED strip.
- basic_Temperature - periodically collects information about the current temperature and/or the relative humidity of the area, every 60 seconds or any other time period if so defined. DHT22 temperature sensor is used as an example.
- basic_TemperatureTarget_OnOff - the thermostat is a device for controlling and regulating indoor temperature. If the current measured room temperature is less than the set temperature in the application, the relay activation burns the heater and warms up the area. Room temperature is measured periodically, and as a result of the desired value, the relay is switched off.
- basic_Motion - when motion is detected, user will get notification from the application, e-mail and/or SMS depending on what is selected in the configuration of the device. In this example, PIR motion sensor triggers alarm if armed in the application. In the same way it is possible to connect any digital sensor. For example: rain sensor, gas, smoke, presence, magnetic sensor...
- basic_multipleEndpoints - periodically collects information about current temperature and/or humidity from several sensors simultaneously in different areas, every 60 seconds or any other time period if so defined. Four DHT22 sensors are used as an example.
- advanced_MultiOnOff - this example allows you to control relay boards and multiple endpoints with digital control signal(on/off), both via Ethernet(Arduino boards) and WiFi.
- advanced_MultiOneWireTemperature - periodically collects information from multiple 1-Wire temperature sensors and displays them every 60 seconds or any other time period if so defined.

Your smart devices can be a combination of multiple endpoints from this library. That way devices can be implemented for virtually every user need.

Make sure you delete the device in the app (can be done in Settings > Manage devices) before you upload the sketch of another device or the current one that underwent significant changes. The app is set to keep the last known configuration, and it may cause trouble during the testing period.

## Other platforms
This platforms will have examples in their own subfolders

