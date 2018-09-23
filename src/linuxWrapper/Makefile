#https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile

EXE = example

OBJ_DIR = obj

INCLUDES = -Ilib/spdlog/include -Ilib -I../ -I../Endpoints

CXX = g++
CXXFLAGS = -g -D LINUX -std=c++14 -Wall -pedantic $(INCLUDES)
LDFLAGS = -lm

SOURCES = $(wildcard *.cpp) \
		  $(wildcard src/*.cpp) \
		  $(wildcard lib/*.cpp) \
		  $(wildcard ../*.cpp) \
		  $(wildcard ../Endpoints/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f *.o lib/*.o src/*.o ../*.o ../Endpoints/*.o