## Makefile for XBeachBall
## author: Ulrike Hager

CXX = g++
CXXFLAGS += -O2 -fPIC -Wall -std=c++11 -I.
DEBUG_FLAGS = -g -DDEBUG 

XLIBS = -lX11

OBJS = XDisplayBase.o XBeachBall.o BeachBall.o

all: $(OBJS) BeachBall

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

.PHONY: clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

BeachBall: $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) $(XLIBS) -o $@

clean:
	rm -f *.o  $(OBJS) 
