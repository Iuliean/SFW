CXX =g++
CFLAGS =-g3 -Wall -Wpedantic -std=c++20
LIBS=-lpthread
INCLUDE=-I$(shell pwd)/include/
INTERMEDIATES= $(shell pwd)/intermediate/
OUTPUT = $(shell pwd)/build/ 

export CFLAGS
export LIBS
export INCLUDE
export INTERMEDIATES
export CXX
export OUTPUT 

.PHONY: all link compile clean

all: compile link

compile:
	$(MAKE) -C src/


link:compile
	$(CXX) -o build/sock $(shell ls $(INTERMEDIATES)*.o) $(CFLAGS) $(LIBS)

clean:
	rm -f intermediate/*