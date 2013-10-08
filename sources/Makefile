# a simple makefile for data check generate the executable

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CC = clang
CXX = clang++
CFLAGS = -g -O0 -std=c++11 \
	-I.. \
	-I/opt/local/include \
	-I/usr/local/include \
	-I/opt/local/include/opencv2
LIBS = -L/opt/local/lib/ \
	-framework GLUT -framework Cocoa -framework OpenGL \
	-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video \
	-lboost_program_options-mt -lboost_system-mt -lboost_iostreams-mt -lboost_filesystem-mt
endif

ifeq ($(UNAME), Linux)
CC = gcc
CXX = g++
CFLAGS = -g -O0 -std=c++11 \
	-I.. \
	-I$(HOME)/include \
	-I/usr/local/include \
	-I/usr/include \
	-I/usr/include/opencv2 \
	-I/usr/lib/x86_64-linux-gnu 
LIBS = \
   -L/usr/lib64/nvidia \
   -L/usr/local/lib \
   -L/usr/local/lib64 \
   -L/usr/lib64 \
	-lboost_program_options -lboost_system -lboost_iostreams -lboost_filesystem \
	-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect \
	-lgsl -lgslcblas -lm
endif

ALL=mola_data_parser

all: $(ALL)

mola_data_parser.o : mola_data_parser.cpp mola_data.hpp
	$(CXX) -o mola_data_parser.o -c mola_data_parser.cpp $(CFLAGS)
glut_win.o : glut_win.cpp glut_win.h
	$(CXX) -o glut_win.o -c glut_win.cpp $(CFLAGS)
win_mola_data.o : win_mola_data.h win_mola_data.cpp glut_win.h
	$(CXX) -o win_mola_data.o -c win_mola_data.cpp $(CFLAGS)

mola_data_parser : mola_data_parser.o glut_win.o win_mola_data.o
	$(CXX) -o mola_data_parser mola_data_parser.o glut_win.o win_mola_data.o $(LIBS)

clean:
	rm -f $(ALL) *.o
