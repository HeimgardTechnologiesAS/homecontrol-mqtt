CXX=g++
CXXFLAGS=-g -std=c++14 -Wall -pedantic

example: main.o
	$(CXX) -o example main.o -O2

debug: main.o
	$(CXX) -o example main.o -O0

.PHONY: clean

clean:
	rm -f *.o