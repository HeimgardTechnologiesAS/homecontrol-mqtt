CXX=g++
CXXFLAGS=-g -D LINUX -std=c++14 -Wall -pedantic
LDFLAGS = -lm

SOURCES = $(wildcard *.cpp) \
		  $(wildcard src/*.cpp) \
		  $(wildcard ../*.cpp) \
		  $(wildcard ../Endpoints/*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

example: ${OBJECTS}
	$(CXX) -o $@ $^ ${CXXFLAGS} ${LDFLAGS} -O2

.PHONY: clean

clean:
	rm -f *.o src/*.o ../*.o ../Endpoints/*.o