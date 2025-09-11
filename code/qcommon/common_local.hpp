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

#ifndef COMMON_LOCAL_HPP_
#define COMMON_LOCAL_HPP_
#pragma once

#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "qcommon.h"
#include "q_shared.h"
#ifdef __cplusplus
}

// color escape character
constexpr int C_COLOR_ESCAPE            = '^';
constexpr int C_COLOR_DEFAULT           = '0';
constexpr int C_COLOR_RED               = '1';
constexpr int C_COLOR_GREEN             = '2';
constexpr int C_COLOR_YELLOW            = '3';
constexpr int C_COLOR_BLUE              = '4';
constexpr int C_COLOR_CYAN              = '5';
constexpr int C_COLOR_ORANGE            = '6';
constexpr int C_COLOR_WHITE             = '7';
constexpr int C_COLOR_GRAY              = '8';
constexpr int C_COLOR_BLACK             = '9';

// color escape string
/*constexpr sz::string S_COLOR_DEFAULT	=	"^0";
constexpr sz::string S_COLOR_RED		=	"^1";
constexpr sz::string S_COLOR_GREEN		=	"^2";
constexpr sz::string S_COLOR_YELLOW		=	"^3";
constexpr sz::string S_COLOR_BLUE		=	"^4";
constexpr sz::string S_COLOR_CYAN		=	"^5";
constexpr sz::string S_COLOR_ORANGE		=	"^6";
constexpr sz::string S_COLOR_WHITE		=	"^7";
constexpr sz::string S_COLOR_GRAY		=	"^8";
constexpr sz::string S_COLOR_BLACK		=	"^9";*/

#define S_COLOR_DEFAULT				"^0"
#define S_COLOR_RED					"^1"
#define S_COLOR_GREEN				"^2"
#define S_COLOR_YELLOW				"^3"
#define S_COLOR_BLUE				"^4"
#define S_COLOR_CYAN				"^5"
#define S_COLOR_ORANGE				"^6"
#define S_COLOR_WHITE				"^7"
#define S_COLOR_GRAY				"^8"
#define S_COLOR_BLACK				"^9"

#endif
#include "common_log.hpp"
#include "utility.hpp"
#include "common_cvar.hpp"
#include "common_cvarsystem.hpp"

using common::cvar::CVar;

#include "../renderer/render_local.hpp"

#include "common_console.hpp"
#include "common_file.hpp"
#include "common_filesystem.hpp"



namespace common
{
	inline std::shared_ptr<CVar> net_allowCheats;
	inline std::shared_ptr<CVar> com_developer;

    extern std::unique_ptr<filesystem::FileSystem> fs;
    extern std::unique_ptr<cvar::CVarSystem> cv;
	extern std::unique_ptr<console::Console> con;
	extern std::unique_ptr<renderer::FontSystem> font;

	template <typename T, typename... Args>
	void Printf(T& fmt, Args&&...args);

	template <typename T>
	void VPrintf(T& fmt, va_list args);

	template <typename T, typename... Args>
	void DPrintf(T& fmt, Args&&...args);

	template <typename T, typename... Args>
	void Warning(T& fmt, Args&&...args);

	template <typename T, typename... Args>
	void DWarning(T& fmt, Args&&...args);

	void PrintWarnings();
	void ClearWarnings(sz::string& reason);

	template <typename T, typename... Args>
	void Error(T& fmt, Args&&...args);
	
	template <typename T, typename... Args>
	void FatalError(T& fmt, Args&&...args);

	void PrintErrors();
	void ClearErrors(sz::string& reason);
}

#endif // !COMMON_LOCAL_HPP_