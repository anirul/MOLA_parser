/*
 * Copyright (c) 2014, Frederic DUBOUCHET
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

#ifndef PLANET_DB_HEADER_DEFINED
#define PLANET_DB_HEADER_DEFINED

namespace planet {

    class topography_db {
    protected :
	sqlite3* db_;
	std::string file_name_;
	bool need_mutex_;
	boost::mutex local_lock_;
	sector_list sector_;
    public :
	topography_db() : db_(NULL), file_name_("") {}
	topography_db(const std::string& file_name);
	virtual ~topography_db();
    public :
	void set_sector(const sector_list& sector) { sector_ = sector; }
	void open(const std::string& file_name);
	void create_table();
	void clear();
	size_t size();
	// interpolate heigth at position
	double get_heigth(const vector3& loc);
	// dump the database!
	void list(std::multimap<uint32_t, vector3>& mm);
    };

} // end namespace planet

#endif // PLANET_DB_HEADER_DEFINED
