#include <iostream>
#include <string>
#include <algorithm>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <opencv.hpp>

#include "mola_data.hpp"
#include "win_interface.hpp"
#include "glut_window.h"
#include "win_mola_data.h"

using namespace boost::program_options;

int main(int ac, char** av) {
    std::string in_file;
    std::string out_file;
    uint32_t pitch;
    try {
	options_description desc("Allowed options");
	desc.add_options
	    ()
	    ("help,h", "produce help message")
	    ("input-file,i", value<std::string>(), "input file (data from MOLA)")
	    ("output-file,o", value<std::string>(), "output file (sub domain 512x512)")
	    ("pitch,p", value<uint32_t>(), "pitch (length of a line in float)");
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
	    std::cout << "output file     : " << out_file << std::endl;
	} else {
	    throw std::runtime_error("no output file");
	}
	if (vm.count("pitch")) {
	    pitch = vm["pitch"].as<uint32_t>();
	    std::cout << "pitch           : " << pitch << std::endl;
	} else {
	    throw std::runtime_error("no pitch");
	}
	{ // main stuff!
	    MOLA::data<float> d(in_file, pitch);
	    uint32_t lines = d.size() / d.get_image_pitch();
	    uint32_t column = d.get_image_pitch();
	    assert(!(lines % 512));
	    assert(!(column % 512));
	    for (uint32_t x = 0; x < column; x += 512) {
		for (uint32_t y = 0; y < lines; y += 512) {
		    std::cout 
			<< "getting patch   : (" << (x / 512) 
			<< ", " << (y / 512) 
			<< ")" << std::endl;
		    std::vector<float> patch = d.get_data_window(
			std::pair<uint32_t, uint32_t>(x, y),
			std::pair<uint32_t, uint32_t>(512, 512));
		    std::stringstream patch_name;
		    patch_name << out_file << (x / 512) << "x" << (y / 512) << ".img";
		    FILE* patch_file = fopen(patch_name.str().c_str(), "wb");
		    if (!patch_file)
			throw std::runtime_error(
			    std::string("Could not open file : ") + patch_name.str());
		    for (int i = 0; i < patch.size(); ++i) {
			uint32_t raw = 0;
			memcpy(&raw, &patch[i], sizeof(float));
			uint32_t value = htonl(raw);
			uint32_t ret = fwrite(&value, sizeof(uint32_t), 1, patch_file);
			if (ret != 1)
			    throw std::runtime_error(
				std::string("Error writting file : ") + patch_name.str());
		    }
		    fclose(patch_file);
		}
	    }
	}
    } catch (std::exception& ex) {
	std::cerr << "exception (std) : " << ex.what() << std::endl;
	return -1;
    }
}
