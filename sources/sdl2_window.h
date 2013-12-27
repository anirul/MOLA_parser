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

#ifndef SDL2_WINDOW_HEADER_DEFINED
#define SDL2_WINDOW_HEADER_DEFINED

namespace win {
    
    // SDL2 window
    class sdl2_window {
    protected :
	SDL_Window* window_;
	std::string name_;
	static std::map<std::string, sdl2_window*>* instances_;
	bool fullscreen_;
	sdl2_window(
	    const std::string& name,
	    const std::pair<unsigned int, unsigned int>& range,
	    interface* windesc,
	    bool fullscreen = false)
	    throw(std::exception);
    public :
	interface* pwin_;
	static sdl2_window* instance(
	    const std::string& name,
	    const std::pair<unsigned int, unsigned int>& range,
	    interface* windesc,
	    bool fullscreen = false)
	    throw(std::exception);
	static sdl2_window* instance(const std::string& name) throw(std::exception);
	virtual ~sdl2_window();
	void run() throw(std::exception);
	// window
	void set_window_display_mode(const SDL_DisplayMode& mode) throw(std::exception);
	SDL_DisplayMode get_window_display_mode() const throw(std::exception);
	float get_window_brightness() const;
	void set_window_brightness(float b);
    };

/*
	// SDL2 specific
	// drivers
	static std::vector<std::string> get_driver_list();
	static std::string get_current_driver();
	static void set_current_driver(const std::string& drv);
	// display
	static std::vector<std::string> get_display_name_list();
	static std::vector<SDL_DisplayMode> get_display_mode_list(
	    const std::string& display) throw(std::exception);
	static SDL_DisplayMode get_current_display_mode(
	    const std::string& display) throw(std::exception);
	static SDL_DisplayMode get_current_desktop_mode(
	    const std::string& display) throw(std::exception);

 */
}

#endif // SDL2_WINDOW_HEADER_DEFINED
