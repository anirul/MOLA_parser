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
 * DISCLAIMED. IN NO EVENT SHALL Frederic DUBOUCHEDT BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GL_CAMERA_HEADER_DEFINED
#define GL_CAMERA_HEADER_DEFINED

namespace gl {

template <typename T>
class camera : public object<T> {
protected :
    vector<T> target_;
    vector<T> up_;
    T fov_;
    T ratio_;
    T near_plane_;
    T far_plane_;
public :
    camera(
	vector<T> pos, 
	vector<T> target, 
	vector<T> up,
	T width,
	T height) : object<T>(pos), target_(target), up_(up) {
	fov_ = (T)45.0;
	ratio_ = (T)(width / height);
	far_plane_ = (T)10000.0;
	near_plane_ = (T)40.0;
	vector<T> right = up_ % (target_ - pos);
	up_ = (target_ - pos) % right;
	up_ = !up_;
    }
    void ratio(int w, int h) {
	ratio_ = (T)w / (T)h;
    }
    void translate(const vector<T>& v) {
	this->pos_ = this->pos_ + v;
	target_ = target_ + v;
    }
    void rotate(const vector<T> v) {
	const T maxz = (T)100;
	const T minz = (T)5;
	// X
	vector<T> pos = this->pos_ - target_;
	matrix<T> rot_y(
	    (T)cos(v.x), (T)0, (T)-sin(v.x),
	    (T)0, (T)1, (T)0,
	    (T)sin(v.x), (T)0, (T)cos(v.x));
	pos = pos * rot_y;
	pos = pos + target_;
	// Z
	vector<T> ta2po = this->pos_ - target_;
	ta2po = !ta2po * (ta2po.length() + v.z);
	if (ta2po.length() > maxz)
	    ta2po = !ta2po * maxz;
	if (ta2po.length() < minz)
	    ta2po = !ta2po * minz;
	// Y
	T t2pl = ta2po.length();
	ta2po.y += v.y;
	ta2po = !ta2po * t2pl;
	this->pos_ = ta2po + target_;
	// recompute up
	up_ = vector<T>((T)0, (T)1, (T)0);
	vector<T> right = up_ % (target_ - this->pos_);
	up_ = (target_ - this->pos_) % right;
	up_ = !up_;
    }
    void center() {
	const T DTOR = (T)0.0174532925;
	T wd2 = near_plane_ * (T)tan((double)(DTOR * (T)fov_ / (T)2));
	T left = -ratio_ * wd2;
	T right = ratio_ * wd2;
	T top = wd2;
	T bottom = -wd2;
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(left, right, bottom, top, near_plane_, far_plane_);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	gluLookAt(
	    this->pos_.x, this->pos_.y, this->pos_.z, 
	    target_.x, target_.y, target_.z,
	    up_.x, up_.y, up_.z);
    }
    vector<T> dir() const {
	vector<T> d((T)0, (T)0, (T)0);
	d = target_ - this->pos_;
	d.y = (T)0;
	d = !d;
	return std::move(d);
    }
    virtual void display() const {}
    virtual std::vector<vector<T> > hit(
	const vector<T>& from, 
	const vector<T>& dir) const {
	return std::move(std::vector<vector<T> >());
    }
};

} // end namespace gl

#endif // GL_CAMERA_HEADER_DEFINED
