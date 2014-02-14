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

#ifndef GL_OBJECT_HEADER_DEFINED
#define GL_OBJECT_HEADER_DEFINED

namespace gl {

    template <typename T>
    struct vector {
	T x;
	T y;
	T z;
	vector(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {} 
	T length() {
	    T l = x * x + y * y + z * z;
	    return (T)sqrt((double)l);
	}
    };

    template <typename T>
    vector<T> operator!(const vector<T>& v) {
	T sq = v * v;
	T inv_sq = (T)1 / (T)sqrt((double)sq);
	return std::move(vector<T>(v.x * inv_sq, v.y * inv_sq, v.z * inv_sq));
    }

    template <typename T>
    T operator* (const vector<T>& v1, const vector<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename T>
    vector<T> operator%(const vector<T>& v1, const vector<T>& v2) {
	return std::move(vector<T>(
			     v1.y * v2.z - v1.z * v2.y,
			     v1.z * v2.x - v1.x * v2.z,
			     v1.x * v2.y - v1.z * v2.x));
    } 

    template <typename T>
    vector<T> operator+(const vector<T>& v1, const vector<T>& v2) {
	return std::move(vector<T>(
			     v1.x + v2.x,
			     v1.y + v2.y,
			     v1.z + v2.z));
    }

    template <typename T>
    vector<T> operator-(const vector<T>& v1, const vector<T>& v2) {
	return std::move(vector<T>(
			     v1.x - v2.x,
			     v1.y - v2.y,
			     v1.z - v2.z));
    }

    template <typename T>
    vector<T> operator*(const vector<T>& v, const T& c) {
	return std::move(vector<T>(v.x * c, v.y * c, v.z * c));
    }

    template <typename T>
    struct matrix {
	T m_[16];
	matrix() {
	    for (int i = 0; i < 16; ++i)
		m_[i] = (T)0;
	}
	matrix(
	    T c11, T c12, T c13,
	    T c21, T c22, T c23,
	    T c31, T c32, T c33)
	    {
		(*this)(0, 0) = c11;
		(*this)(0, 1) = c12;
		(*this)(0, 2) = c13;
		(*this)(0, 3) = (T)0;
	
		(*this)(1, 0) = c21;
		(*this)(1, 1) = c22;
		(*this)(1, 2) = c23;
		(*this)(1, 3) = (T)0;
	
		(*this)(2, 0) = c31;
		(*this)(2, 1) = c32;
		(*this)(2, 2) = c33;
		(*this)(2, 3) = (T)0;

		(*this)(3, 0) = (T)0;
		(*this)(3, 1) = (T)0;
		(*this)(3, 2) = (T)0;
		(*this)(3, 3) = (T)1;
	    }
	matrix(
	    T c11, T c12, T c13, T c14,
	    T c21, T c22, T c23, T c24,
	    T c31, T c32, T c33, T c34,
	    T c41, T c42, T c43, T c44)
	    {
		(*this)(0, 0) = c11;
		(*this)(0, 1) = c12;
		(*this)(0, 2) = c13;
		(*this)(0, 3) = c14;
	
		(*this)(1, 0) = c21;
		(*this)(1, 1) = c22;
		(*this)(1, 2) = c23;
		(*this)(1, 3) = c24;
	
		(*this)(2, 0) = c31;
		(*this)(2, 1) = c32;
		(*this)(2, 2) = c33;
		(*this)(2, 3) = c34;

		(*this)(3, 0) = c41;
		(*this)(3, 1) = c42;
		(*this)(3, 2) = c43;
		(*this)(3, 3) = c44;
	    }
	static matrix<T> identity() {
	    matrix<T> m;
	    for (int i = 0; i < 4; ++i) {
		m(i, i) = (T)1;
	    }
	    return std::move(m);
	}
	T& operator()(uint32_t x, uint32_t y) {
	    assert(x < 4);
	    assert(y < 4);
	    return m_[x + (y * 4)];
	}
	T operator()(uint32_t x, uint32_t y) const {
	    assert(x < 4);
	    assert(y < 4);
	    return m_[x + (y * 4)];
	}
    };

// vector * matrix -> vector
    template<typename T>
    vector<T> operator*(const vector<T> v, const matrix<T>& m) {
	return std::move(vector<T>(
			     v.x * m(0, 0) + v.y * m(0, 1) + v.z * m(0, 2),
			     v.x * m(1, 0) + v.y * m(1, 1) + v.z * m(1, 2),
			     v.z * m(2, 0) + v.y * m(2, 1) + v.z * m(2, 2)));
    } 

    template <typename T>
    class object {
    protected :
	vector<T> pos_;
    public :
	object(const vector<T>& pos) : pos_(pos) {}
	virtual void set_pos(const vector<T>& v) { pos_ = v; }
	virtual vector<T> get_pos() const { return std::move(pos_); }
	virtual void display() const = 0;
	virtual std::vector<vector<T> > hit(
	    const vector<T>& from, 
	    const vector<T>& dir) const = 0;
    };

} // end namespace gl

#endif // GL_OBJECT_HEADER_DEFINED
