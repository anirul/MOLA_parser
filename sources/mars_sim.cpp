/*
 * Copyright (c) 2013, Frederic Dubouchet
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the CERN nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Frederic Dubouchet ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Frederic DUBOUCHET BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <string>
#include <algorithm>

#ifdef __linux__
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __APPLE__
#define GL3_PROTOTYPES
#include <OpenGL/gl3.h>
#define __gl_h_
#include <OpenGL/glu.h>
#include <glut/glut.h>
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <opencv.hpp>

#include "mola_data.hpp"
#include "win_interface.hpp"
#include "glut_window.h"
#include "gl_object.hpp"
#include "gl_camera.hpp"
#include "gl_sphere.hpp"
#include "win_mars.h"

using namespace boost::program_options;
using namespace boost::posix_time;

int main(int ac, char** av) {
    bool is_fullscreen = false;
    uint32_t width = 1024;
    uint32_t height = 768;
    try {
	options_description desc("Allowed options");
	desc.add_options
	    ()
	    ("help,h", "produce help message")
	    ("fullscreen,f", "allow fullscreen resolution")
	    ("width,w", value<uint32_t>(), "width of the screen or window")
	    ("height,h", value<uint32_t>(), "height of the screen or window")
	    ;
	variables_map vm;
	store(command_line_parser(ac, av).options(desc).run(), vm);
	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    return 1;
	}
	if (vm.count("fullscreen")) {
	    is_fullscreen = true;
	}
	if (vm.count("width")) {
	    width = vm["width"].as<uint32_t>();
	}
	if (vm.count("height")) {
	    height = vm["height"].as<uint32_t>();
	}
	win_mars wm(std::make_pair(width, height));
        win::glut_window* pwin = win::glut_window::instance(
	    std::string("Mars simulation"),
	    std::make_pair(width, height),
	    &wm,
	    is_fullscreen);
	pwin->run();
    } catch (std::exception& ex) {
	std::cerr << "exception (std) : " << ex.what() << std::endl;
	return -1;
    }
    return 0;
}
