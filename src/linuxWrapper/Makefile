#https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile

EXE = example

OBJ_DIR = obj

INCLUDES = -Ilib/spdlog/include -Ilib -Isrc -I../ -I../Endpoints

CXX = g++
CXXFLAGS = -g -ggdb -D LINUX -std=c++14 -Wall -pedantic $(INCLUDES)
LDFLAGS = -g -ggdb -lm -L/usr/local/lib -lmosquittopp

SOURCES = $(wildcard *.cpp) \
		  $(wildcard src/*.cpp) \
		  $(wildcard ../*.cpp) \
		  $(wildcard ../Endpoints/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f *.o lib/*.o src/*.o ../*.o ../Endpoints/*.o