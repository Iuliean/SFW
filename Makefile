CXX =g++
CFLAGS =-g3 -Wall -Wpedantic -std=c++20 -fPIC
LIBS=-lpthread
WORKDIR=$(shell pwd)
DEPENDENCIES=$(WORKDIR)/dependencies

INCLUDE=-I$(WORKDIR)/include/ \
		-I$(DEPENDENCIES)/spdlog/include

INTERMEDIATES= $(WORKDIR)/intermediate/
OUTPUT = $(WORKDIR)/build/ 

export CFLAGS
export LIBS
export INCLUDE
export INTERMEDIATES
export CXX
export OUTPUT 

.PHONY: all link compile clean

all: compile link

compile:
	mkdir -p $(OUTPUT)
	mkdir -p $(INTERMEDIATES)
	$(MAKE) -C src/


link:compile
	$(CXX) -shared -o build/libSFW.so $(shell ls $(INTERMEDIATES)*.o) $(CFLAGS) $(LIBS)

clean:
	rm -f intermediate/*