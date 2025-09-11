/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//

/*
quake3 common
2025 Bleeding Eye Studios
*/

#ifndef LOG_HPP_
#define LOG_HPP_
#pragma once

#include "common_local.hpp"

constexpr auto LOG_FILE_NAME = "%y%m%d_%H%M.log";
constexpr auto LOG_PATH = "logs/";

#define LOG(level)	BOOST_LOG_TRIVIAL(level)
#define LOG_FUNC()	__pragma(warning(disable:4365))		/*Signed/unsigned mismatch*/		\
					/*BOOST_LOG_NAMED_SCOPE(__func__)*/BOOST_LOG_NAMED_SCOPE(__FUNCTION__)	\
					__pragma(warning(default:4365))		/*Signed/unsigned mismatch*/

enum severity_level
{
	trace,
	debug,
	info,
	warning,
	error,
	fatal
};

namespace common
{
	class Log
	{
	private:
		bool	m_initialized = false;

	public:
		Log();
	};

	extern std::unique_ptr<Log> log;
}

#endif // !LOG_HPP_