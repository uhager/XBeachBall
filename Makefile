## Makefile for XBeachBall
## author: Ulrike Hager

CXX = g++
CXXFLAGS += -O2 -fPIC -Wall -std=c++11 -I. 
DEBUG_FLAGS = -g -DDEBUG 

LIBS = -lX11 -pthread

OBJS = XDisplayBase.o XBeachBall.o XDrawAverages.o BeachBall.o

all: $(OBJS) BeachBall

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

.PHONY: clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

BeachBall: $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -f *.o  $(OBJS) 
