#include <iostream>
#include <math.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/program_options.hpp>
#include <boost/interprocess/sync/lock_options.hpp>

#include <iostream>
#include <vector>

#include <sqlite3.h>

#include "planet_vector3.h"
#include "planet_sector.h"
#include "planet_db.h"
#include "mola_data.hpp"

namespace map_type {
    typedef map_type uint32_t;
    const uint32_t polar = 0x10;
    const uint32_t cylindric = 0x20;
    const uint32_t north = 0x1;
    const uint32_t south = 0x2;
    const uint32_t polar_north = 0x11;
    const uint32_t polar_south = 0x12;
    const uint32_t cylindric_north = 0x21;
    const uint32_t cylindric_south = 0x22;
    const uint32_t undefined = 0x0;
}

int main(int ac, char** av) {
    std::string input_file = "";
    uint32_t pitch = 0;
    double theta = 0.0;
    double phi = 0.0;
    double delta_theta = 0.0;
    double delta_phi = 0.0;
    double polar_max = 0.0;
    map_type::map_type mt = map_type::undefined;
    try {
	options_description desc("Allowed options");
	desc.add_options
	    ()
	    ("help,h", "produce help message")
	    ("polar", "polar map type")
	    ("cylindric", "cylindric map type")
	    ("north,n", "north map")
	    ("south,s", "south map")
	    ("input-file,i", value<std::string>(), "input file (img)")
	    ("pitch", value<uint32_t>(), "pitch of the img")
	    ("theta,t", value<double>(), "theta (in radian) longitude")
	    ("phi,f", value<double>(), "phi (in radian) latitude")
	    ("delta-theta", value<double>(), "delta theta (in radian) longitude")
	    ("delta-phi", value<double>(), "delta phi (in radian) latitude")
	    ("polar-max,m", value<double>(), "max (or min) phi of a polar map");
	variables_map vm;
	store(command_line_parser(ac, av).options(desc).run(), vm);
	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    return 1;
	}
	// case (polar | cylindric)
	if (vm.count("polar")) {
	    if (vm.count("north") && !vm.count("south")) {
		mt = map_type::polar_north;
	    } else if (vm.count("south") && !vm.count("north")) {
		mt = map_type::polar_south;
	    } else {
		throw std::runtime_error("map can either be north or south");
	    }
	} else if (vm.count("cylindric")) {
	    if (vm.count("north") && !vm.count("south")) {
		mt = map_type::cylindric_north;
	    } else if (vm.count("south") && !vm.count("north")) {
		mt = map_type::cylindric_south;
	    } else {
		throw std::runtime_error("map can either be north or south");
	    }
	} else {
	    throw std::runtime_error("map can either be polar or cylindric");
	}
	// check for bounderies
	if (mt & map_type::cylindric) {
	    if (vm.count("theta") && 
		vm.count("phi") && 
		vm.count("delta-theta") &&
		vm.count("delta-phi"))
	    {
		phi = vm["phi"].as<double>();
		theta = vm["theta"].as<double>();
		delta_theta = vm["delta-theta"].as<double>();
		delta_phi = vm["delta-phi"].as<double>();
	    } else {
		throw std::runtime_error("cylindric map must have a phi theta and the deltas");
	    }
	}
	if (mt & map_type::polar) {
	    if (vm.count("polar-max")) {
		polar_max = vm["polar-max"].as<double>();
	    } else {
		throw std::runtime_error("polar map must have a max(min)");
	    }
	}
	if (vm.count("input-file")) {
	    input_file = vm["input-file"].as<std::string>();
	} else {
	    throw std::runtime_error("need an input file");
	}
	if (vm.count("pitch")) {
	    pitch = vm["pitch"].as<uint32_t>();
	} else {
	    throw std::runtime_error("need a pitch to decode the image");
	}
    } catch (std::exception& ex) {
	std::cerr << "exception (std) : " << ex.what() << std::endl;
	return -1;
    }
    return 0;
}
