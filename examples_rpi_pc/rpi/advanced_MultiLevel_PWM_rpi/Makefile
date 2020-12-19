#https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile
EXE = hc-mqtt

OBJ_DIR = obj

INCLUDES = -Ihomecontrol-mqtt/wrappers/linuxWrapper/lib/spdlog/include \
           -Ihomecontrol-mqtt/wrappers/linuxWrapper/lib/arrrgh \
		   -Ihomecontrol-mqtt/wrappers/linuxWrapper/lib \
		   -Ihomecontrol-mqtt/wrappers/linuxWrapper/src \
		   -Ihomecontrol-mqtt/src -Ihomecontrol-mqtt/src/Endpoints \
		   -Ihomecontrol-mqtt/wrappers

CXX = g++
CXXFLAGS = -g -O0 -ggdb3 -D LINUX  -D DEBUG_ALL -std=c++17 $(INCLUDES)
LDFLAGS = -g -O0 -ggdb3 -lm -L/usr/local/lib -lmosquittopp -lwiringPi

SOURCES = $(wildcard *.cpp) \
		  $(wildcard homecontrol-mqtt/wrappers/linuxWrapper/src/*.cpp) \
		  $(wildcard homecontrol-mqtt/src/*.cpp) \
		  $(wildcard homecontrol-mqtt/src/Endpoints/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	cd homecontrol-mqtt
	git clean -fdx
	cd ..
	rm -f *.o
