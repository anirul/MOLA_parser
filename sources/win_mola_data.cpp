#include <stdexcept>
#include <string>
#include <vector>
#ifdef __linux__
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __APPLE__
#include <glut/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include <boost/date_time/posix_time/posix_time.hpp>
#include <opencv.hpp>

#include "mola_data.hpp"
#include "win_interface.hpp"
#include "glut_window.h"
#include "win_mola_data.h"

win_mola_data::win_mola_data(
    const std::pair<uint32_t, uint32_t>& range,
    MOLA::data_interface* di) :
    range_(range),
    texture_id_(0),
    data_interface_(di),
    pos_(std::make_pair<uint32_t, uint32_t>(0, 0))
{}

win_mola_data::~win_mola_data() {}

void win_mola_data::init() {
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(-1, 1, -1, 1);
    glGenTextures(1, &texture_id_);
}

void win_mola_data::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(-1, 1);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(1, 0);
    glVertex2f(1, -1);
    glTexCoord2f(0, 0);
    glVertex2f(-1, -1);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glFlush();
    glutPostRedisplay();
}

void win_mola_data::idle() {
    glFinish();
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
	GL_TEXTURE_2D, 
	0, 
	1, 
	data_interface_->get_image_pitch(), 
	10240,
 	0, 
	GL_LUMINANCE, 
	GL_FLOAT, 
        data_interface_->get_image_pointer());
    glFinish();
}

void win_mola_data::reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glFinish();
}

void win_mola_data::mouse_event(int button, int state, int x, int y) {
    if ((button == 3) || (button == 4)) // it's a wheel event
    {
	if (button == 3) {
	    // Wheel up
	}
	if (button == 4) {
	    // Wheel down
	}
    }
}

void win_mola_data::mouse_move(int x, int y) {}

void win_mola_data::special(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
	// up
	pos_.first += 10;
    }
    if (key == GLUT_KEY_DOWN) {
	// down
	pos_.first -= 10;
    }
    if (key == GLUT_KEY_RIGHT) {
	// right
	pos_.second += 10;
    }
    if (key == GLUT_KEY_LEFT) {
	// left
	pos_.second -= 10;
    }
}

void win_mola_data::keyboard(unsigned char key, int x, int y) {}

void win_mola_data::finish() {}
