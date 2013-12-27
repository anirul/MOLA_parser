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

#include <SDL.h>
#include <map>
#include <string>
#include "win_interface.hpp"
#include "sdl2_window.h"

std::map<std::string, win::sdl2_window*>* win::sdl2_window::instances_ = NULL;

win::sdl2_window::sdl2_window(
    const std::string& name,
    const std::pair<unsigned int, unsigned int>& range,
    win::interface* windesc,
    bool fullscreen)
    throw(std::exception) {
    
}

win::sdl2_window* win::sdl2_window::instance(
    const std::string& name,
    const std::pair<unsigned int, unsigned int>& range,
    win::interface* windesc,
    bool fullscreen) throw(std::exception) {
    return NULL;
}

win::sdl2_window* win::sdl2_window::instance(const std::string& name) 
    throw(std::exception) {
    return NULL;
}

win::sdl2_window::~sdl2_window() {}

void win::sdl2_window::run() throw(std::exception) {}

void win::sdl2_window::set_window_display_mode(
    const SDL_DisplayMode& mode) throw(std::exception) {}

SDL_DisplayMode win::sdl2_window::get_window_display_mode() const throw(std::exception) {
    SDL_DisplayMode sdm;
    memset(&sdm, 0, sizeof(SDL_DisplayMode));
    return std::move(sdm);
}
