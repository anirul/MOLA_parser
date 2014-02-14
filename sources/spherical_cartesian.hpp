#ifndef SPHERICAL_CARTESIAN
#define SPHERICAL_CARTESIAN

struct vector3 {
    static vector3 from_spherical(double radius, double theta, double phi) {
	vector3 vec3;
	vec3.x = radius * sin(theta) * cos(phi);
	vec3.y = radius * sin(theta) * sin(phi);
	vec3.z = radius * cos(theta);
	return vec3;
    }
    double x;
    double y;
    double z;
};

#endif // SPHERICAL_CARTESIAN
