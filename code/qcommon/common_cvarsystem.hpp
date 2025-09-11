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
quake3 client
2025 Bleeding Eye Studios
*/

#ifndef COMMON_CVARSYSTEM_HPP_
#define COMMON_CVARSYSTEM_HPP_
#pragma once

#include "common_local.hpp"

namespace common
{
	namespace cvar
	{
		class CVar;

		bool			Cvar_InfoValidate(const sz::string& value);

		class CVarSystem
		{
		private:
			bool m_initialized = false;

			std::unordered_map<sz::string, std::shared_ptr<CVar>> m_CVarMap;

		public:
			bool						Initialize();
			void						Shutdown();

			[[nodiscard]] bool IsInitialized() const { return m_initialized; }

			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, bool defaultValue, unsigned int flags, const sz::string& description);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, int64_t defaultValue, unsigned int flags, const sz::string& description, int64_t valueMin = INT64_MIN, int64_t valueMax = INT64_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, uint64_t defaultValue, unsigned int flags, const sz::string& description, uint64_t valueMin = 0, uint64_t valueMax = UINT64_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, int defaultValue, unsigned int flags, const sz::string& description, int valueMin = INT_MIN, int valueMax = INT_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, unsigned int defaultValue, unsigned int flags, const sz::string& description, unsigned int valueMin = 0, unsigned int valueMax = UINT_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, float defaultValue, unsigned int flags, const sz::string& description, float valueMin = FLT_MIN, float valueMax = FLT_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, double defaultValue, unsigned int flags, const sz::string& description, double valueMin = DBL_MIN, double valueMax = DBL_MAX);
			[[nodiscard]] std::shared_ptr<CVar> Register(const sz::string& name, const sz::string& defaultString, unsigned int flags, const sz::string& description);

			[[nodiscard]] std::shared_ptr<CVar> Find(const sz::string& name);
			[[nodiscard]] std::shared_ptr<CVar> Find(const char* name);

			[[nodiscard]] std::vector<std::shared_ptr<CVar>> FindAll(const sz::string& searchRegex, unsigned int flags) const;

			void ListToConsole(const sz::string& searchRegex = "", unsigned int flags = 0) const;
		};
	}
}

#endif // !COMMON_CVARSYSTEM_HPP_