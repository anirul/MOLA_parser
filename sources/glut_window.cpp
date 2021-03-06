/*
 * Copyright (c) 2012, Frederic Dubouchet
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

#include <stdexcept>
#include <string>
#ifdef __linux__
#include <GL/glut.h>
#endif
#ifdef __APPLE__
#define GL3_PROTOTYPES
#include <OpenGL/gl3.h>
#define __gl_h_
#include <glut/glut.h>
#endif
#include <boost/bind.hpp>
#include <stdlib.h>

#include "win_interface.hpp"
#include "glut_window.h" 

static void init() {
    win::glut_window::instance()->pwin_->init();
}

static void finish() {
    win::glut_window::instance()->pwin_->finish();
}

static void display() {
    static bool s_first = true;
    if (s_first) {
	s_first = false;
	init();
    } else {
	win::glut_window::instance()->pwin_->display();
    }
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int x, int y) {
    switch(key) {
    case 27 :
	finish();
	exit(0);
	break;
    }
    win::glut_window::instance()->pwin_->keyboard(key, x, y);
}

static void special(int key, int x, int y) {
    win::glut_window::instance()->pwin_->special(key, x, y);
}

static void reshape(int w, int h) {
    win::glut_window::instance()->pwin_->reshape(w, h);
}

static void idle() {
    win::glut_window::instance()->pwin_->idle();
    glutPostRedisplay();
}

static void mouse_move(int x, int y) {
    win::glut_window::instance()->pwin_->mouse_move(x, y);
}

static void mouse_event(int button, int state, int x, int y) {
    win::glut_window::instance()->pwin_->mouse_event(button, state, x, y);
}

win::glut_window* win::glut_window::instance_ = NULL;

win::glut_window* win::glut_window::instance(
    const std::string& name,
    const std::pair<unsigned int, unsigned int>& range, 
    win::interface* windesc, 
    bool fullscreen) 
    throw(std::exception)
{
    if (!instance_)
	instance_ = new win::glut_window(name, range, windesc, fullscreen);
    return instance_;
}

win::glut_window* win::glut_window::instance() throw(std::exception) {
    if (instance_) return instance_;
    else throw std::runtime_error(std::string("win::glut_window was not initialized!"));
}

win::glut_window::glut_window(
    const std::string& name,
    const std::pair<unsigned int, unsigned int>& range, 
    win::interface* windesc, 
    bool fullscreen) 
    throw(std::exception)
{
    if (!windesc) throw std::runtime_error(std::string("Window class == NULL"));
    pwin_ = windesc;
    fullscreen_ = fullscreen;
    int ac = 0;
    glutInit(&ac, NULL);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    if (!fullscreen_)
	glutInitWindowSize(range.first, range.second);
    glutCreateWindow(name.c_str());
    if (fullscreen_)
	glutFullScreen();
}

win::glut_window::~glut_window() { }

void win::glut_window::run() 
    throw(std::exception)
{
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse_event);
    glutIdleFunc(idle);
    glutMainLoop();
}

