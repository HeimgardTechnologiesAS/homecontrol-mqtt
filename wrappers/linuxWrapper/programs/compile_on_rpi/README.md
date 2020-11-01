
# Docker
You can read more about docker on docker.com. It is not necessary that you know the details, but you need to set up docker to run on your system. This is different from distribution to distribution, so please follow the help pages for your distribution.

There are several cross compile docker containers, but most does not really do what we want it to do. The simplest approach is to run a container that trough qemu runs the native raspberry pi os.

# Setting up the environment
Assuming that you keep your git repositories in your ~/git folder, also run the follwing to get the homecontrol mqtt library:
git clone https://github.com/HomeControlAS/homecontrol-mqtt.git --depth=1 ~/git/homecontrol-mqtt
cd ~/git/homecontrol-mqtt
git submodule update --init

# Building a project
Copy the content of this to a folder where you would like to keep your project. Running the compile.sh file will start the docker container with the correctly mounted folders and compile the contents of the folder according to the makefile.