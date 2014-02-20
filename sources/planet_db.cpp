
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

#include <string>
#include <map>
#include <utility>

#include <boost/thread.hpp>
#include <boost/interprocess/sync/lock_options.hpp>

#include <sqlite3.h>

#include "planet_vector3.h"
#include "planet_sector.h"
#include "planet_db.h"

namespace planet {

    topography_db::topography_db(const std::string& file_name)
	: db_(NULL), file_name_(file_name)
    {
	open(file_name);
    }

    topography_db::~topography_db() {
	try {
	    boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	    if (need_mutex_) local_lock_.lock();
	    sqlite3_close(db_);
	    db_ = NULL;
	} catch(std::exception& ex) {
	    local_lock_.unlock();
	    throw ex;
	}
    }

    void topography_db::open(const std::string& file_name) {
	file_name_ = file_name;
	int rc = 0;
	need_mutex_ = !sqlite3_threadsafe();
	if (need_mutex_) {
	    boost::mutex::scoped_lock lock_it(local_lock_);
	    rc = sqlite3_open(file_name_.c_str(), &db_);
	    if (rc) {
		std::stringstream error("");
		error << "Can't open database : ";
		error << sqlite3_errmsg(db_);
		sqlite3_close(db_);
		db_ = NULL;
		local_lock_.unlock();
		throw std::runtime_error(error.str());
	    }
	} else {
	    rc = sqlite3_open_v2(
		file_name_.c_str(),
		&db_,
		SQLITE_OPEN_READWRITE |
		SQLITE_OPEN_CREATE |
		SQLITE_OPEN_FULLMUTEX |
		SQLITE_OPEN_SHAREDCACHE,
		NULL);
	    if (rc) {
		std::stringstream error("");
		error << "Can't open database (full mutex, err = " << rc << " : ";
		error << sqlite3_errmsg(db_);
		sqlite3_close(db_);
		db_ = NULL;
		throw std::runtime_error(error.str());
	    }
	}
	create_table();
    }

    void topography_db::create_table() {
	boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	if (need_mutex_) local_lock_.lock();
	int rc = 0;
	char* szErrMsg = 0;
	std::string sql_query = 
	    "PRAGMA foreign_keys = ON;"\
	    "CREATE TABLE IF NOT EXISTS "\
	    "location(id INTEGER PRIMARY KEY AUTOINCREMENT, "\
	    "x DOUBLE, y DOUBLE, z DOUBLE);"\
	    "CREATE TABLE IF NOT EXISTS "\
	    "sector_reference(id INTEGER, "\
	    "ref INTEGER, "\
	    "FOREIGN KEY (id) REFERENCES location(id) "\
	    "ON DELETE CASCADE);";
	rc = sqlite3_exec(
	    db_,
	    sql_query.c_str(),
	    NULL,
	    0,
	    &szErrMsg);
	if (rc != SQLITE_OK) {
	    std::stringstream ss("");
	    ss << "SQL error in CREATE TABLE : ";
	    ss << szErrMsg;
	    sqlite3_free(szErrMsg);
	    local_lock_.unlock();
	    throw std::runtime_error(ss.str());
	}
    }

    void topography_db::clear() {
	int rc = 0;
	char* szErrMsg = 0;
	{
	    boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	    if (need_mutex_) local_lock_.lock();
	    rc = sqlite3_exec(
		db_,
		"DROP TABLE IF EXISTS location",
		NULL,
		0,
		&szErrMsg);
	    if (rc != SQLITE_OK) {
		std::stringstream ss("");
		ss << "SQL error in DROP TABLE : ";
		ss << szErrMsg;
		sqlite3_free(szErrMsg);
		local_lock_.unlock();
		throw std::runtime_error(ss.str());
	    }
	}
	{
	    boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	    if (need_mutex_) local_lock_.lock();
	    rc = sqlite3_exec(
		db_,
		"DROP TABLE IF EXISTS sector_reference",
		NULL,
		0,
		&szErrMsg);
	    if (rc != SQLITE_OK) {
		std::stringstream ss("");
		ss << "SQL error in DROP TABLE : ";
		ss << szErrMsg;
		sqlite3_free(szErrMsg);
		local_lock_.unlock();
		throw std::runtime_error(ss.str());
	    }
	}
    }

    size_t topography_db::size() {
	boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	if (need_mutex_) local_lock_.lock();
	int rc = 0;
	char** result;
	int nrow, ncol;
	char* szMsg;
	rc = sqlite3_get_table(
	    db_,
	    "SELECT Count(*) FROM location",
	    &result,
	    &nrow,
	    &ncol,
	    &szMsg);
	if (rc != SQLITE_OK) {
	    std::stringstream ss("");
	    ss << "SQL error in SELECT (size) : ";
	    ss << szMsg;
	    sqlite3_free(szMsg);
	    sqlite3_free_table(result);
	    local_lock_.unlock();
	    throw std::runtime_error(ss.str());
	}
	if (ncol != 1) return 0;
	if (nrow != 1) return 0;
	return (size_t)atoi(result[1]);
    }

    double topography_db::get_heigth(const vector3& loc) {
	if (sector_.size() == 0) throw std::runtime_error("no sector defined!");
	// find the sector point closest to the location vector
	uint32_t loc_ref = sector_.get_closest_sector(loc);
	// extract all the point in the DB that are in the same sector
	std::vector<vector3> sector_locations;
	{ // access DB
	    boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	    if (need_mutex_) local_lock_.lock();
	    int rc = 0;
	    sqlite3_stmt* stmt = NULL;
	    { // let's build the string
		std::stringstream ss("");
		ss << "SELECT location.x, location.y, location.z ";
		ss << "FROM location, sector_references ";
		ss << "WHERE location.id IN ( ";
		ss << "SELECT location.id ";
		ss << "FROM location, sector_references ";
		ss << "WHERE location.id = sector_references.id IN ( ";
		ss << "SELECT sector_references.id ";
		ss << "FROM sector_references ";
		ss << "WHERE sector_references.ref = '" << loc_ref << "'))";
		rc = sqlite3_prepare_v2(
		    db_,
		    ss.str().c_str(),
		    -1,
		    &stmt,
		    NULL);
	    }
	    if (rc != SQLITE_OK) {
		std::stringstream ss("");
		ss << "SQL error in SELECT (get_heigth) : ";
		ss << sqlite3_errmsg(db_);
		local_lock_.unlock();
		throw std::runtime_error(ss.str());
	    }
	    // for each row
	    while (sqlite3_step(stmt) == SQLITE_ROW) {
		// for each column
		vector3 location;
		for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
		    std::string column_name = sqlite3_column_name(stmt, i);
		    if (column_name == std::string("x"))
			location.x = sqlite3_column_double(stmt, i);
		    if (column_name == std::string("y"))
			location.y = sqlite3_column_double(stmt, i);
		    if (column_name == std::string("z"))
			location.z = sqlite3_column_double(stmt, i);
		}
		sector_locations.push_back(location);
	    }
	    rc = sqlite3_finalize(stmt);
	    if (rc != SQLITE_OK) {
		local_lock_.unlock();
		throw std::runtime_error("SQL error finalize in SELECT (get_heigth)");
	    }
	}
	// search the points closest to location in the returned array
	double closest_value = -1.0;
	vector3 closest_loc;
	vector3 loc_normal = loc.normal();
	for (vector3 loc_ite : sector_locations)
	    if ((loc_ite.normal() * loc_normal) > closest_value)
		closest_loc = loc_ite;
	// return the interpolated heigth	
	return closest_loc.radius();
    }

    void topography_db::list(std::multimap<uint32_t, vector3>& mm) {
	boost::mutex::scoped_lock lock_it(local_lock_, boost::defer_lock);
	if (need_mutex_) local_lock_.lock();
	int rc = 0;
	sqlite3_stmt* stmt = NULL;
	rc = sqlite3_prepare_v2(
	    db_,
	    "SELECT sector_references.ref, location.x, location.y location.z "\
	    "FROM sector_references, location "\
	    "WHERE sector_references.id = location.id",
	    -1,
	    &stmt,
	    NULL);
	if (rc != SQLITE_OK) {
	    std::stringstream ss("");
	    ss << "SQL error in SELECT (list) : ";
	    ss << sqlite3_errmsg(db_);
	    local_lock_.unlock();
	    throw std::runtime_error(ss.str());
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
	    uint32_t sector_ref;
	    vector3 location;
	    for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
		std::string column_name = sqlite3_column_name(stmt, i);
		if (column_name == std::string("ref"))
		    sector_ref = sqlite3_column_int(stmt, i);
		if (column_name == std::string("x"))
		    location.x = sqlite3_column_double(stmt, i);
		if (column_name == std::string("y"))
		    location.y = sqlite3_column_double(stmt, i);
		if (column_name == std::string("z"))
		    location.z = sqlite3_column_double(stmt, i);
	    }
	    mm.insert(std::make_pair(sector_ref, location));
	}
	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
	    local_lock_.unlock();
	    throw std::runtime_error("SQL error finalize in SELECT (list)");
	}
    }

} // end namespace planet
