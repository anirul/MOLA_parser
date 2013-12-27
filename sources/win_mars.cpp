#include <stdexcept>
#include <string>
#include <vector>
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
#include <opencv.hpp>

#include "mola_data.hpp"
#include "win_interface.hpp"
#include "glut_window.h"
#include "gl_object.hpp"
#include "gl_camera.hpp"
#include "gl_sphere.hpp"
#include "win_mars.h"

win_mars::win_mars(const std::pair<uint32_t, uint32_t>& range) :
    camera_(NULL),
    range_(range),
    pos_(0, 0)
{}

win_mars::~win_mars() {
    if (camera_) delete camera_;
    if (sphere_) delete sphere_;
}

void win_mars::init() {
    glClearColor(0, 0, 0, 0);
    camera_ = new gl::camera<double>(
	gl::vector<double>(0.0, 45.0, 75.0),
	gl::vector<double>(0.0, 30.0, 0.0),
	gl::vector<double>(0.0, 1.0, 0.0),
	range_.first,
	range_.second);
}

void win_mars::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (GLsizei)range_.first, (GLsizei)range_.second);
    if (camera_) {
	camera_->ratio(range_.first, range_.second);
	camera_->center();
    }
//    glPushMatrix();
    if (sphere_) {
	sphere_->display();
    }
//    glPopMatrix();

    glFlush();
    glutPostRedisplay();
}

void win_mars::idle() {}

void win_mars::reshape(int w, int h) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    range_.first = w;
    range_.second = h;
}

void win_mars::mouse_event(int button, int state, int x, int y) {}

void win_mars::mouse_move(int x, int y) {}

void win_mars::special(int key, int x, int y) {}

void win_mars::keyboard(unsigned char key, int x, int y) {}

void win_mars::finish() {}
