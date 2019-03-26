#https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile

EXE = example

OBJ_DIR = obj

INCLUDES = -I../../lib/spdlog/include -I../../lib/arrrgh -I../../lib -I../../src -I../../../../src -I../../../ -I../../../../src/Endpoints

CXX = g++
CXXFLAGS = -g -O0 -ggdb3 -D LINUX -std=c++14 -Wall -pedantic $(INCLUDES)
LDFLAGS = -g -O0 -ggdb3 -lm -L/usr/local/lib -lmosquittopp

SOURCES = $(wildcard *.cpp) \
		  $(wildcard ../../src/*.cpp) \
		  $(wildcard ../../../*.cpp) \
		  $(wildcard ../../../Endpoints/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f *.o ../../lib/*.o ../../src/*.o ../../../*.o ../../../Endpoints/*.o