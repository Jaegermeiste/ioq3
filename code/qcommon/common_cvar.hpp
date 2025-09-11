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

#ifndef COMMON_CVAR_HPP_
#define COMMON_CVAR_HPP_
#pragma once

#include "common_local.hpp"

namespace common
{
	namespace cvar
	{
		enum CVarType
		{
			CVAR_TYPE_NONE,
			CVAR_TYPE_BOOL,
			CVAR_TYPE_INT64,
			CVAR_TYPE_UINT64,
			CVAR_TYPE_INT,
			CVAR_TYPE_UINT,
			CVAR_TYPE_FLOAT,
			CVAR_TYPE_DOUBLE,
			CVAR_TYPE_STRING
		};

		class CVarSystem;

		class CVar : public std::enable_shared_from_this<CVar>
		{
		private:
			friend class CVarSystem;

			std::shared_ptr<cvar_t> m_legacyPtr = nullptr;

			std::mutex				m_ptrAccessMutex;

			void SyncLegacyFlag(int flag, bool overwriteLegacy = false);

			sz::string       m_name;
			sz::string       m_description;
			sz::string       m_valueString;
			sz::string       m_latchedString;
			sz::string       m_resetString;
			double           m_minValue = DBL_MIN;
			double           m_maxValue = DBL_MAX;
			unsigned int     m_flags;

			CVarType         m_variableType = CVAR_TYPE_NONE;

			void SyncLegacy(bool overwriteLegacy = false);

		protected:
			CVar(const sz::string& name, bool defaultValue, unsigned int flags, const sz::string& description);
			CVar(const sz::string& name, int64_t defaultValue, unsigned int flags, const sz::string& description, int64_t valueMin = INT64_MIN, int64_t valueMax = INT64_MAX);
			CVar(const sz::string& name, uint64_t defaultValue, unsigned int flags, const sz::string& description, uint64_t valueMin = 0, uint64_t valueMax = UINT64_MAX);
			CVar(const sz::string& name, int defaultValue, unsigned int flags, const sz::string& description, int valueMin = INT_MIN, int valueMax = INT_MAX);
			CVar(const sz::string& name, unsigned int defaultValue, unsigned int flags, const sz::string& description, unsigned int valueMin = 0, unsigned int valueMax = UINT_MAX);
			CVar(const sz::string& name, float defaultValue, unsigned int flags, const sz::string& description, float valueMin = FLT_MIN, float valueMax = FLT_MAX);
			CVar(const sz::string& name, double defaultValue, unsigned int flags, const sz::string& description, double valueMin = DBL_MIN, double valueMax = DBL_MAX);
			CVar(const sz::string& name, const sz::string& defaultString, unsigned int flags, const sz::string& description);

		public:
			~CVar();

			[[nodiscard]] bool         Bool();
			[[nodiscard]] int64_t      Int64();
			[[nodiscard]] uint64_t     UInt64();
			[[nodiscard]] signed int   Int();
			[[nodiscard]] unsigned int UInt();
			[[nodiscard]] float        Float();
			[[nodiscard]] double       Double();
			[[nodiscard]] sz::string   String();
			[[nodiscard]] sz::string   LatchedString();
			[[nodiscard]] sz::string   Name();
			[[nodiscard]] unsigned int Flags();
			[[nodiscard]] sz::string   Description();
			[[nodiscard]] bool         Modified();
			[[nodiscard]] CVarType     Type() const;

			[[nodiscard]] sz::string   InfoString();

			void			Set(const sz::string& string, const bool force = false);
			void			Set(const bool value, const bool force = false);
			void			Set(const int64_t value, const bool force = false);
			void			Set(const uint64_t value, const bool force = false);
			void			Set(const signed int value, const bool force = false);
			void			Set(const unsigned int value, const bool force = false);
			void			Set(const float value, const bool force = false);
			void			Set(const double value, const bool force = false);
			void            SetFlag(const unsigned int flag);
			void            SetFlags(const unsigned int flags, const bool updateNew = true);
			void            ClearFlag(const unsigned int flag);
			void			SetModified();
			void			ClearModified();

			void            Reset();

			void            Unlatch(bool force = false);

			[[nodiscard]] cvar_t* Legacy();
		};
	};
};

#endif // !COMMON_CVAR_HPP_