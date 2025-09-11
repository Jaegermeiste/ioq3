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

#include "common_local.hpp"

namespace common
{
	static std::vector<sz::string> warningList;
	static std::vector<sz::string> errorList;
}

template <typename T, typename... Args>
void common::Printf(T& fmt, Args&&...args)
{
	sz::string formatted_string = Format(fmt, std::forward<Args>(args)...);

	LOG(info) << formatted_string;

	con->Print(formatted_string);
}

template <typename T>
void common::VPrintf(T& fmt, va_list args)
{
	char formatted_cstring[4096]{'\0'};

	va_start(args, fmt);
	vsprintf(formatted_cstring, ToString(fmt).c_str(), args);
	va_end(args);

	LOG(info) << formatted_cstring;

	con->Print(formatted_cstring);
}

template <typename T, typename... Args>
void common::DPrintf(T& fmt, Args&&...args)
{
	if (!com_developer || !com_developer->Bool())
	{
		return;			// don't confuse non-developers with techie stuff...
	}

	sz::string formatted_string = Format(fmt, std::forward<Args>(args)...);

	LOG(debug) << formatted_string;

	con->Print(sz::string(S_COLOR_ORANGE) + "DEBUG: " + formatted_string);
}

template <typename T, typename... Args>
void common::Warning(T& fmt, Args&&...args)
{
	sz::string formatted_string = Format(fmt, std::forward<Args>(args)...);

	LOG(warning) << formatted_string;

	con->Print(sz::string(S_COLOR_YELLOW) + "WARNING: " + formatted_string);

	warningList.push_back(formatted_string);
}

template <typename T, typename... Args>
void common::DWarning(T& fmt, Args&&...args)
{
	if (!com_developer || !com_developer->Bool())
	{
		return;			// don't confuse non-developers with techie stuff...
	}

	Warning(fmt, std::forward<Args>(args)...);
}

template <typename T, typename... Args>
void common::Error(T& fmt, Args&&...args)
{
	sz::string formatted_string = Format(fmt, std::forward<Args>(args)...);

	LOG(error) << formatted_string;

	con->Print(sz::string(S_COLOR_RED) + "ERROR: " + formatted_string);

	errorList.push_back(formatted_string);

	//Sys_Error(StripColors(formatted_string).c_str());
}

template <typename T, typename... Args>
void common::FatalError(T& fmt, Args&&...args)
{
	sz::string formatted_string = Format(fmt, std::forward<Args>(args)...);

	LOG(fatal) << formatted_string;

	con->Print(sz::string(S_COLOR_RED) + "FATAL: " + formatted_string);

	errorList.push_back(formatted_string);

	Sys_Error(StripColors(formatted_string).c_str());
}

void common::PrintWarnings()
{
	if (warningList.empty())
	{
		return;
	}

	Printf("------------- Warnings ---------------\n");

	for (auto& warning : warningList)
	{
		con->Print(sz::string(S_COLOR_YELLOW) + "WARNING: " + warning);
	}

	Printf("%d warnings\n", warningList.size());
}

void common::ClearWarnings(sz::string& reason)
{
	LOG_FUNC();

	LOG(debug) << "Clearing Warnings List: " << reason;

	warningList.clear();
}

void common::PrintErrors()
{
	if (errorList.empty())
	{
		return;
	}

	Printf("------------- Errors ---------------\n");

	for (auto& error : errorList)
	{
		con->Print(sz::string(S_COLOR_RED) + "ERROR: " + error);
	}

	Printf("%d errors\n", errorList.size());
}

void common::ClearErrors(sz::string& reason)
{
	LOG_FUNC();

	LOG(debug) << "Clearing Error List: " << reason;

	errorList.clear();
}










