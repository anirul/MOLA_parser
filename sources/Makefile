# a simple makefile for data check generate the executable

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CC = clang
CXX = clang++
CFLAGS = -g -O0 -std=c++11 \
	-I.. \
	-I/opt/local/include \
	-I/usr/local/include \
	-I/opt/local/include/opencv2 \
	-I/opt/local/include/SDL2
LIBS = -L/opt/local/lib/ \
	-framework GLUT -framework Cocoa -framework OpenGL \
	-lSDL2 \
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

ALL=mola_data_parser mars_sim mars_cut mars_spherical_cartesian mars_map2db

all: $(ALL)

mola_data_parser.o : mola_data_parser.cpp mola_data.hpp
	$(CXX) -o mola_data_parser.o -c mola_data_parser.cpp $(CFLAGS)
mars_sim.o : mars_sim.cpp win_mars.h gl_object.hpp gl_camera.hpp gl_sphere.hpp
	$(CXX) -o mars_sim.o -c mars_sim.cpp $(CFLAGS)
glut_window.o : glut_window.cpp glut_window.h win_interface.hpp
	$(CXX) -o glut_window.o -c glut_window.cpp $(CFLAGS)
sdl2_window.o : sdl2_window.cpp sdl2_window.h win_interface.hpp
	$(CXX) -o sdl2_window.o -c sdl2_window.cpp $(CFLAGS)
win_mola_data.o : win_mola_data.h win_mola_data.cpp glut_window.h win_interface.hpp
	$(CXX) -o win_mola_data.o -c win_mola_data.cpp $(CFLAGS)
win_mars.o : win_mars.h win_mars.cpp glut_window.h gl_object.hpp gl_camera.hpp gl_sphere.hpp win_interface.hpp
	$(CXX) -o win_mars.o -c win_mars.cpp $(CFLAGS)
mars_cut.o : mars_cut.cpp mola_data.hpp
	$(CXX) -o mars_cut.o -c mars_cut.cpp $(CFLAGS)
mars_spherical_cartesian.o : mars_spherical_cartesian.cpp spherical_cartesian.hpp mola_data.hpp
	$(CXX) -o mars_spherical_cartesian.o -c mars_spherical_cartesian.cpp $(CFLAGS)
mars_map2db.o : mars_map2db.cpp spherical_cartesian.hpp mola_data.hpp
	$(CXX) -o mars_map2db.o -c mars_map2db.cpp $(CFLAGS)

mola_data_parser : mola_data_parser.o glut_window.o win_mola_data.o
	$(CXX) -o mola_data_parser mola_data_parser.o glut_window.o win_mola_data.o $(LIBS)
mars_sim : glut_window.o win_mars.o mars_sim.o
	$(CXX) -o mars_sim glut_window.o win_mars.o mars_sim.o $(LIBS)
mars_cut : mars_cut.o
	$(CXX) -o mars_cut mars_cut.o $(LIBS)
mars_spherical_cartesian : mars_spherical_cartesian.o
	$(CXX) -o mars_spherical_cartesian mars_spherical_cartesian.o $(LIBS)
mars_map2db : mars_map2db.o
	$(CXX) -o mars_map2db mars_map2db.o $(LIBS)

clean:
	rm -f $(ALL) *.o *~
