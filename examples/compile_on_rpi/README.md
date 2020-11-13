# Running on rPi
Go the the folder on your rPi that you want to use to build and run this command:  
```
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/setup.sh)
```
This will install the necessary packages, clone the git repositories needed, and copy a Makefile and a main.cpp file that you can work from. Now you only need to run 'make' to build the example.  
  
Alternately use this command to also setup the software to start on boot and make the initial build for all libaries and the executable
```
source <(curl -s https://raw.githubusercontent.com/HomeControlAS/homecontrol-mqtt/develop/examples/compile_on_rpi/full_setup.sh)
```

# Docker
You can read more about docker on docker.com. It is not necessary that you know the details, but you need to set up docker to run on your system. This is different from distribution to distribution, so please follow the help pages for your distribution.

There are several cross compile docker containers, but most does not really do what we want it to do. The simplest approach is to run a container that trough qemu runs the native raspberry pi os, and mount the folder from above
