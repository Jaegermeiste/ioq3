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

#ifndef COMMON_CONSOLE_HPP_
#define COMMON_CONSOLE_HPP_
#pragma once

#include "common_local.hpp"

namespace common
{
	namespace console
	{
		typedef enum justify_e
		{
			JUSTIFY_LEFT,
			JUSTIFY_RIGHT,
			JUSTIFY_CENTER_LEFT,
			JUSTIFY_CENTER_RIGHT,
			JUSTIFY_CENTER
		} justify_t;

		typedef struct consoleText_s
		{
			sz::string		text;
			justify_t		justify;
			time_t			time;
		} consoleText_t;

		constexpr size_t CONSOLE_LINES_SIZE = 0x30000;
		constexpr size_t COMMAND_HISTORY_SIZE = 64;
		constexpr size_t OVERLAY_SIZE = 3;

		class Console
		{
		private:
			bool m_hasFocus = false;

			boost::circular_buffer<consoleText_t> m_mainRingBuffer;
			boost::circular_buffer<consoleText_t> m_commandHistoryRingBuffer;
			boost::circular_buffer<consoleText_t> m_overlayRingBuffer;

			void Print(const consoleText_t& consoleText);
			void PrintOverlay(const consoleText_t& consoleText);

		public:
			bool						Initialize();
			void						Shutdown();

			bool IsActive() const { return m_hasFocus; }

			void Open(const float openFraction = 1.0);
			void Close();

			void Draw(const bool fullScreen);

			void Print(const sz::string& string, const justify_t justify = JUSTIFY_LEFT);
			void PrintOverlay(const sz::string& string, const justify_t justify = JUSTIFY_LEFT);

			void Clear();
		};
	}
}

#endif // !COMMON_CONSOLE_HPP_