/*
 * Copyright (c) 2014, anirul
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
 * THIS SOFTWARE IS PROVIDED BY Frederic DUBOUCHET ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Frederic DUBOUCHEDT BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PLANET_VECTOR_HEADER_DEFINED
#define PLANET_VECTOR_HEADER_DEFINED

namespace planet {

    class vector3 {
    public :
	vector3() { x = 0.0; y = 0.0; z = 0.0; }
	vector3(double x_, double y_, double z_)
	    : x(x_), y(y_), z(z_) {}
	vector3 normal() const {
	    double r = radius();
	    vector3 n;
	    n.x = x / r;
	    n.y = y / r;
	    n.z = z / r;
	    return std::move(n);
	}
	double phi() const {
	    return atan(y / x);
	}
	double theta() const {
	    return acos(z / radius());
	}
	double radius() const {
	    return sqrt(x * x + y * y + z * z);
	}
	double x;
	double y;
	double z;
    };

    double operator*(const vector3& v1, const vector3& v2);
    vector3 vector3_from_spherical(double theta, double phi, double radius = 1.0);
	
} // end namespace planet

#endif // PLANET_VECTOR_HEADER_DEFINED
