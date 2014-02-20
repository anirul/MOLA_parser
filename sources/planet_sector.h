
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
 * THIS SOFTWARE IS PROVIDED BY anirul ``AS IS'' AND ANY
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

#ifndef PLANET_SECTOR_HEADER_DEFINED
#define PLANET_SECTOR_HEADER_DEFINED

namespace planet {

    class sector_list {
    protected :
	std::vector<vector3> sector_list_;
    public :
	void add_sector(const vector3& v) { sector_list_.push_back(v.normal()); }
	size_t size() const { return sector_list_.size(); }
	uint32_t get_closest_sector(const vector3& v) {
	    uint32_t closest_id = 0;
	    double closest_value = -1.0;
	    vector3 norm_v = v.normal();
	    for (uint32_t ite = 0; ite < size(); ++ite) {
		double current_value = sector_list_[ite] * norm_v;
		if (current_value > closest_value) closest_id = ite;
	    }
	    return closest_id;
	}
    };

    sector_list sector_oct();
    sector_list sector_ico();

} // end namespace planet

#endif // PLANET_SECTOR_HEADER_DEFINED
