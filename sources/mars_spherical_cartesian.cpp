#include <iostream>
#include <math.h>

#include <boost/math/constants/constants.hpp>
#include <boost/program_options.hpp>

#include "planet_vector3.h"

using namespace boost::program_options;

int main(int ac, char** av) {
    const double pi = boost::math::constants::pi<double>();
    const double deg2radian = pi / 180.0;
    double radius = 1.0;
    double theta = 0.0;
    double phi = 0.0;
    try {
	options_description desc("Allowed options");
	desc.add_options
	    ()
	    ("help,h", "produce help message")
	    ("north,n", value<double>(), "north coordinates")
	    ("south,s", value<double>(), "south coordinates")
	    ("east,e", value<double>(), "east coordinates")
	    ("west,w", value<double>(), "west coordinates")
	    ("radius,r", value<double>(), "radius");
	variables_map vm;
	store(command_line_parser(ac, av).options(desc).run(), vm);
	if (vm.count("help")) {
	    std::cout << desc << std::endl;
	    return 1;
	}
	if (vm.count("north") && vm.count("south"))
	    throw std::runtime_error("cannot be both North and South!");
	if (vm.count("east") && vm.count("west"))
	    throw std::runtime_error("cannot be both East and West!");
	if (vm.count("north")) {
	    theta = deg2radian * (90.0 - vm["north"].as<double>());
	} else if (vm.count("south")) {
	    theta = deg2radian * (90.0 + vm["south"].as<double>());
	}
	std::cout << "theta           : " << theta << std::endl; 
	if (vm.count("east")) {
	    phi = deg2radian * vm["east"].as<double>();
	} else if (vm.count("west")) {
	    phi = deg2radian * (360.0 - vm["west"].as<double>());
	}
	std::cout << "phi             : " << phi << std::endl;
	if (vm.count("radius")) {
	    radius = vm["radius"].as<double>();
	}
	std::cout << "radius          : " << radius << std::endl;
	planet::vector3 vec3 = planet::vector3_from_spherical(theta, phi, radius);
	std::cout << "vec3.x          : " << vec3.x << std::endl;
	std::cout << "vec3.y          : " << vec3.y << std::endl;
	std::cout << "vec3.z          : " << vec3.z << std::endl;
	std::cout << "computed radius : " <<
	    sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z)
		  << std::endl;
    } catch (std::exception& ex) {
	std::cerr << "exception (std) : " << ex.what() << std::endl;
	return -1;
    }
    return 0;
}
