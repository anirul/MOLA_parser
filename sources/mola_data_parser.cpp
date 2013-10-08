#include <iostream>
#include <string>
#include <algorithm>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <opencv.hpp>

#include "mola_data.hpp"
#include "glut_win.h"
#include "win_mola_data.h"

using namespace boost::program_options;
using namespace boost::posix_time;

int main(int ac, char** av) {
    std::string in_file;
    std::string out_file = "out.png";
    uint32_t element_size = 4;
    bool is_real = true;
    bool is_output_file = false;
    uint32_t pitch = 0;
    try {
	options_description desc("Allowed options");
	desc.add_options
	    ()
	    ("help,h", "produce help message")
	    ("input-file,i", value<std::string>(), "input file (data from MOLA)")
	    ("output-file,o", value<std::string>(), "output file (image)")
	    ("int32", "data is integer 32 bits")
	    ("int16", "data is integer 16 bits")
	    ("int8", "data is integer 8 bits")
	    ("float32", "data is float 32 bits")
	    ("pitch", value<uint32_t>(), "pitch (length of a line)");
	variables_map vm;
	store(command_line_parser(ac, av).options(desc).run(), vm);
	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    return 1;
	}
	if (vm.count("input-file")) {
	    in_file = vm["input-file"].as<std::string>();
	    std::cout << "input file      : " << in_file << std::endl; 
	} else {
	    throw std::runtime_error("no input file");
	}
	if (vm.count("output-file")) {
	    out_file = vm["output-file"].as<std::string>();
	    is_output_file = true;
	    std::cout << "output file     : " << out_file << std::endl;
	}
	if (vm.count("int32")) {
	    element_size = 4;
	    is_real = false;
	}
	if (vm.count("int8")) {
	    element_size = 1;
	    is_real = false;
	}
	if (vm.count("float32")) {
	    element_size = 4;
	    is_real = true;
	}
	if (vm.count("int16")) {
	    element_size = 2;
	    is_real = false;
	}
	if (vm.count("pitch")) {
	    pitch = vm["pitch"].as<uint32_t>();
	    std::cout << "pitch           : " << pitch << std::endl;
	} else {
	    throw std::runtime_error("no pitch");
	}
	// the real stuff
	if (is_output_file) { // output to file
	    if (is_real && (element_size == 4)) {
		MOLA::data<float> d(in_file, pitch);
		d.save_image(out_file);
	    } else if (!is_real && (element_size == 4)) {
		MOLA::data<int32_t> d(in_file, pitch);
		d.save_image(out_file);
	    } else if (!is_real && (element_size == 2)) {
		MOLA::data<int16_t> d(in_file, pitch);
		d.save_image(out_file);
	    } else {
		throw std::runtime_error("undefined type (1)");
	    }
	} else { // output to screen
	    if (is_real && (element_size == 4)) {
		MOLA::data<float> d(in_file, pitch);
		win_mola_data wmd(std::make_pair(1024, 1024), &d);
		glut_win* pwin = glut_win::instance(
		    std::string("MOLA data view"),
		    std::make_pair(1024, 1024),
		    &wmd,
		    false);
		pwin->run();
	    } else if (!is_real && (element_size == 4)) {
		MOLA::data<int32_t> d(in_file, pitch);
		win_mola_data wmd(std::make_pair(1024, 1024), &d);
		glut_win* pwin = glut_win::instance(
		    std::string("MOLA data view"),
		    std::make_pair(1024, 1024),
		    &wmd,
		    false);
		pwin->run();
	    } else if (!is_real && (element_size == 2)) {
		MOLA::data<int16_t> d(in_file, pitch);
		win_mola_data wmd(std::make_pair(1024, 1024), &d);
		glut_win* pwin = glut_win::instance(
		    std::string("MOLA data view"),
		    std::make_pair(1024, 1024),
		    &wmd,
		    false);
		pwin->run();
	    } else {
		throw std::runtime_error("undefined type (2)");
	    }
	}
    } catch (std::exception& ex) {
	std::cerr <<     "exception (std) : " << ex.what() << std::endl;
	return -1;
    }
    return 0;
}
