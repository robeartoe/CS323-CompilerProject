#Makefile

CXX=g++
CXXFLAGS=-std=c++1y -Wall -pedantic
PROG=-o rat
SOURCES=main.cpp parser.cpp
HEADERS=parser.h

all: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(PROG) $(SOURCES)

debug: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -g -o rat_debug $(SOURCES)

clean:
	rm -f rat rat_debug





