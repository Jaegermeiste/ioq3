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

extern std::shared_ptr<CVar> net_allowCheats;

inline void CVar::SyncLegacyFlag(const int flag, const bool overwriteLegacy)
{
	LOG_FUNC();

	if (overwriteLegacy)
		SyncFlag(m_flags, m_legacyPtr->flags, flag);
	else
		SyncFlag(m_legacyPtr->flags, m_flags, flag);
}

void CVar::SyncLegacy(const bool overwriteLegacy)
{
	LOG_FUNC();

	try
	{
		if (m_legacyPtr)
		{
			if (m_legacyPtr->name == nullptr)
			{
				m_legacyPtr->name = new char[m_name.length() + 1] {'\0'};
				sz::memcpy(m_legacyPtr->name, m_name.begin(), m_name.length());
			}

			m_legacyPtr->next = nullptr;

			if (overwriteLegacy)
			{
				// New takes precedence over Legacy
				if (m_legacyPtr->string != nullptr)
				{
					delete[] m_legacyPtr->string;
					m_legacyPtr->string = nullptr;
				}

				if (m_legacyPtr->string == nullptr)
				{
					m_legacyPtr->string = new char[m_valueString.length() + 1] {'\0'};
					sz::memcpy(m_legacyPtr->string, m_valueString.begin(), m_valueString.length());
				}

				if (!m_latchedString.empty() && !m_latchedString.is_space())
				{
					if (m_legacyPtr->latched_string != nullptr)
					{
						delete[] m_legacyPtr->latched_string;
						m_legacyPtr->latched_string = nullptr;
					}

					if (m_legacyPtr->latched_string == nullptr)
					{
						m_legacyPtr->latched_string = new char[m_latchedString.length() + 1]{'\0'};
						sz::memcpy(m_legacyPtr->latched_string, m_latchedString.begin(), m_latchedString.length());
					}
				}
				else if(m_legacyPtr->latched_string != nullptr)
				{
					delete[] m_legacyPtr->latched_string;
					m_legacyPtr->latched_string = nullptr;
				}

				m_legacyPtr->value = boost::lexical_cast<float>(m_valueString);

				// Iterate through flags one-by-one
				SyncLegacyFlag(CVAR_ARCHIVE, true);
				SyncLegacyFlag(CVAR_USERINFO, true);
				SyncLegacyFlag(CVAR_SERVERINFO, true);
				SyncLegacyFlag(CVAR_NOSET, true);
				SyncLegacyFlag(CVAR_LATCH, true);

				SyncLegacyFlag(CVAR_ROM, true);
				SyncLegacyFlag(CVAR_CHEAT, true);

				SyncLegacyFlag(CVAR_SYSTEM, true);
				SyncLegacyFlag(CVAR_RENDERER, true);
				SyncLegacyFlag(CVAR_SOUND, true);
				SyncLegacyFlag(CVAR_GUI, true);
				SyncLegacyFlag(CVAR_GAME, true);
				SyncLegacyFlag(CVAR_TOOL, true);

				m_legacyPtr->modified = static_cast<qboolean>(Modified());
			}
			else
			{
				// Legacy takes precedence over New
				if (m_legacyPtr->string != nullptr)
				{
					m_valueString = m_legacyPtr->string;
				}

				if (m_legacyPtr->latched_string != nullptr)
				{
					m_latchedString = m_legacyPtr->latched_string;
				}

				m_legacyPtr->value = boost::lexical_cast<float>(m_valueString);

				// Iterate through flags one-by-one
				SyncLegacyFlag(CVAR_ARCHIVE, false);
				SyncLegacyFlag(CVAR_USERINFO, false);
				SyncLegacyFlag(CVAR_SERVERINFO, false);
				SyncLegacyFlag(CVAR_NOSET, false);
				SyncLegacyFlag(CVAR_LATCH, false);

				SyncLegacyFlag(CVAR_ROM, true);
				SyncLegacyFlag(CVAR_CHEAT, true);

				SyncLegacyFlag(CVAR_SYSTEM, true);
				SyncLegacyFlag(CVAR_RENDERER, true);
				SyncLegacyFlag(CVAR_SOUND, true);
				SyncLegacyFlag(CVAR_GUI, true);
				SyncLegacyFlag(CVAR_GAME, true);
				SyncLegacyFlag(CVAR_TOOL, true);
												
				m_legacyPtr->modified ? SetModified() : ClearModified();
			}
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

common::cvar::CVar::CVar(const sz::string& name, const bool defaultValue, const unsigned int flags, const sz::string& description)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_BOOL;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(defaultValue);
	m_minValue = 0.0;
	m_maxValue = 1.0;
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const int64_t defaultValue, const unsigned int flags, const sz::string& description, const int64_t valueMin, const int64_t valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_INT64;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = static_cast<double>(valueMin);
	m_maxValue = static_cast<double>(valueMax);
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const uint64_t defaultValue, const unsigned int flags, const sz::string& description, const uint64_t valueMin, const uint64_t valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_UINT64;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = static_cast<double>(valueMin);
	m_maxValue = static_cast<double>(valueMax);
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const int defaultValue, const unsigned int flags, const sz::string& description, const int valueMin, const int valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_INT;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = valueMin;
	m_maxValue = valueMax;
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const unsigned int defaultValue, const unsigned int flags, const sz::string& description, const unsigned int valueMin, const unsigned int valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_UINT;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = valueMin;
	m_maxValue = valueMax;
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const float defaultValue, const unsigned int flags, const sz::string& description, const float valueMin, const float valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_FLOAT;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = valueMin;
	m_maxValue = valueMax;
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const double defaultValue, const unsigned int flags, const sz::string& description, const double valueMin, const double valueMax)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_DOUBLE;
	m_flags = flags;
	m_description = description;
	m_resetString = boost::lexical_cast<sz::string>(std::ranges::clamp(defaultValue, valueMin, valueMax));
	m_minValue = valueMin;
	m_maxValue = valueMax;
	Set(defaultValue, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::CVar(const sz::string& name, const sz::string& defaultString, const unsigned int flags, const sz::string& description)
{
	LOG_FUNC();

	m_name = name;
	m_variableType = CVAR_TYPE_STRING;
	m_flags = flags;
	m_description = description;
	m_resetString = defaultString;
	m_minValue = 0;
	m_maxValue = 0;
	Set(defaultString, true);
	ClearModified();

	LOG(info) << "Created CVar: " << InfoString();
}

common::cvar::CVar::~CVar()
{
	LOG_FUNC();

	try
	{
		if (m_legacyPtr)
		{
			if (m_legacyPtr->string != nullptr)
			{
				delete[] m_legacyPtr->string;
				m_legacyPtr->string = nullptr;
			}

			if (m_legacyPtr->string != nullptr)
			{
				delete[] m_legacyPtr->string;
				m_legacyPtr->string = nullptr;
			}

			if (m_legacyPtr->latched_string != nullptr)
			{
				delete[] m_legacyPtr->latched_string;
				m_legacyPtr->latched_string = nullptr;
			}

			m_legacyPtr->flags = 0;
			m_legacyPtr->modified = false;
			m_legacyPtr->next = nullptr;

			m_legacyPtr = nullptr;
		}

		m_name.clear();
		m_variableType = CVAR_TYPE_NONE;
		m_flags = 0;
		m_description.clear();
		m_valueString.clear();
		m_latchedString.clear();
		m_resetString.clear();
		m_minValue = 0.0;
		m_maxValue = 0.0;

	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

bool common::cvar::CVar::Bool()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<bool>(m_valueString);
}

int64_t common::cvar::CVar::Int64()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<int64_t>(m_valueString);
}

uint64_t common::cvar::CVar::UInt64()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<uint64_t>(m_valueString);
}

int common::cvar::CVar::Int()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<int>(m_valueString);
}

unsigned int common::cvar::CVar::UInt()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<unsigned int>(m_valueString);
}

float common::cvar::CVar::Float()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<float>(m_valueString);
}

double common::cvar::CVar::Double()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return boost::lexical_cast<double>(m_valueString);
}

sz::string common::cvar::CVar::String()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return m_valueString;
}

sz::string common::cvar::CVar::LatchedString()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return m_latchedString;
}

sz::string common::cvar::CVar::Name()
{
	LOG_FUNC();

	return m_name;
}

unsigned int common::cvar::CVar::Flags()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	return m_flags;
}

sz::string common::cvar::CVar::Description()
{
	LOG_FUNC();

	return m_description;
}

bool common::cvar::CVar::Modified()
{
	LOG_FUNC();

	if (m_legacyPtr)
	{
		SyncLegacy();
	}

	if (m_flags & CVAR_MODIFIED)
	{
		return true;
	}
	return false;
}

common::cvar::CVarType CVar::Type() const
{
	LOG_FUNC();

	return m_variableType;
}

sz::string common::cvar::CVar::InfoString()
{
	LOG_FUNC();

	try
	{
		sz::string attribs;
		sz::string subsystem;
		sz::string type;
		sz::string range;
		sz::string legacy;

		if (IsFlagSet(m_flags, CVAR_ARCHIVE))
			attribs += "A";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_USERINFO))
			attribs += "U";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_SERVERINFO))
			attribs += "S";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_NOSET))
			attribs += "-";
		else if (IsFlagSet(m_flags, CVAR_LATCH))
			attribs += "L";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_ROM))
			attribs += "R";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_CHEAT))
			attribs += "C";
		else
			attribs += " ";

		if (IsFlagSet(m_flags, CVAR_MODIFIED))
			attribs += "M";
		else
			attribs += " ";

		// Subsystem
		if (IsFlagSet(m_flags, CVAR_SYSTEM))
		{
			subsystem += "SYS ";
		}
		else if (IsFlagSet(m_flags, CVAR_RENDERER))
		{
			subsystem += "RNDR";
		}
		else if (IsFlagSet(m_flags, CVAR_SOUND))
		{
			subsystem += "SND ";
		}
		else if (IsFlagSet(m_flags, CVAR_GUI))
		{
			subsystem += "GUI ";
		}
		else if (IsFlagSet(m_flags, CVAR_GAME))
		{
			subsystem += "GAME";
		}
		else if (IsFlagSet(m_flags, CVAR_TOOL))
		{
			subsystem += "TOOL";
		}
		else
		{
			subsystem += "    ";
		}

		switch (m_variableType)
		{
		case CVAR_TYPE_BOOL:
			type = "BOOL";
			break;
		case CVAR_TYPE_UINT64:
			type = "U64";
			break;
		case CVAR_TYPE_INT64:
			type = "I64 ";
			break;
		case CVAR_TYPE_UINT:
			type = "U32";
			break;
		case CVAR_TYPE_INT:
			type = "I32";
			break;
		case CVAR_TYPE_FLOAT:
			type = "FLT";
			break;
		case CVAR_TYPE_DOUBLE:
			type = "DBL";
			break;
		case CVAR_TYPE_STRING:
			type = "STR";
			break;
		case CVAR_TYPE_NONE:
		default:
			type = "UNK ";
		}

		if (m_variableType != CVAR_TYPE_STRING)
		{
			auto zero_set = sz::char_set({ '0' });

			range = "Range: [";
			range += boost::lexical_cast<sz::string>(m_minValue).rstrip(zero_set);
			range += ":";
			range += boost::lexical_cast<sz::string>(m_maxValue).rstrip(zero_set);
			range += "] inclusive. ";
		}

		if (m_legacyPtr)
		{
			legacy = "Mapped to legacy cvar_t";
		}

		return Format("{:7} {:4} {:16} {:4} {:18} {}{} {}", attribs, subsystem, m_name, type, m_valueString, range, m_description, legacy).rstrip(sz::whitespaces_set());
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
	return {};
}

void common::cvar::CVar::Set(const sz::string& string, const bool force)
{
	LOG_FUNC();

	try
	{
		if (!string.empty() && !string.is_space() && (string.compare(m_valueString) != 0))
		{
			if (IsFlagSet(m_flags, CVAR_USERINFO) || IsFlagSet(m_flags, CVAR_SERVERINFO))
			{
				if (!Cvar_InfoValidate(string))
				{
					Error("Invalid info CVar value\n");
					return;
				}
			}

			if (IsFlagSet(m_flags, CVAR_ROM))
			{
				if (!force)
				{
					Warning("%s is read only.\n", m_name.c_str());
					return;
				}
			}

			if (IsFlagSet(m_flags, CVAR_NOSET))
			{
				if (!force)
				{
					Warning("%s is write protected and can only be set from the cmdline (or autoexec.cfg).\n", m_name.c_str());
					return;
				}
			}

			if (IsFlagSet(m_flags, CVAR_CHEAT))
			{
				if (!force)
				{
					bool multiplayer = false;

					if (Com_ServerState() > 0)
					{
						// Not ss_dead, map is (potentially) loaded
						if (cv)
						{
							auto cvar_maxClients = cv->Find("maxclients");

							if (cvar_maxClients && (cvar_maxClients->UInt() > 1))
							{
								multiplayer = true;
							}
						}
					}

					if (multiplayer && net_allowCheats && net_allowCheats->Bool())
					{
						Warning("%s is a cheat.\n", m_name.c_str());
					}
					else if (multiplayer && net_allowCheats && !net_allowCheats->Bool())
					{
						Warning("Cheat CVar %s cannot be changed: cheats are disabled.\n", m_name.c_str());
						return;
					}
				}
			}

			if (IsFlagSet(m_flags, CVAR_LATCH))
			{
				if (Com_ServerState() && !force)
				{
					Printf("%s will be changed for next game.\n", m_name.c_str());
					m_latchedString = string;
				}
				else
				{
					m_valueString = string;
					m_latchedString.clear();
				}
			}
			else
			{
				m_valueString = string;
				m_latchedString.clear();
			}

			::SetFlag(m_flags, CVAR_MODIFIED);

			if (m_legacyPtr)
			{
				SyncLegacy(true);
			}
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

void common::cvar::CVar::Set(const bool value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(value), force);
}

void common::cvar::CVar::Set(const int64_t value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, static_cast<int64_t>(m_minValue), static_cast<int64_t>(m_maxValue))), force);
}

void common::cvar::CVar::Set(const uint64_t value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, static_cast<uint64_t>(m_minValue), static_cast<uint64_t>(m_maxValue))), force);
}

void common::cvar::CVar::Set(const signed int value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, msl::utilities::SafeInt<signed int>(static_cast<int64_t>(m_minValue)).Ref(), msl::utilities::SafeInt<signed int>(static_cast<int64_t>(m_maxValue)).Ref())), force);
}

void common::cvar::CVar::Set(const unsigned int value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, msl::utilities::SafeInt<unsigned int>(static_cast<int64_t>(m_minValue)).Ref(), msl::utilities::SafeInt<unsigned int>(static_cast<int64_t>(m_maxValue)).Ref())), force);
}

void common::cvar::CVar::Set(const float value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, static_cast<float>(m_minValue), static_cast<float>(m_maxValue))), force);
}

void common::cvar::CVar::Set(const double value, const bool force)
{
	LOG_FUNC();

	Set(boost::lexical_cast<sz::string>(std::ranges::clamp(value, m_minValue, m_maxValue)), force);
}

void CVar::SetFlag(const unsigned int flag)
{
	LOG_FUNC();

	::SetFlag(m_flags, flag);

	SetModified();
}

void CVar::SetFlags(const unsigned int flags, const bool updateNew)
{
	LOG_FUNC();

	// Go Flag by Flag
	SyncFlag(flags, m_flags, CVAR_ARCHIVE);
	SyncFlag(flags, m_flags, CVAR_USERINFO);
	SyncFlag(flags, m_flags, CVAR_SERVERINFO);
	SyncFlag(flags, m_flags, CVAR_NOSET);
	SyncFlag(flags, m_flags, CVAR_LATCH);

	if (updateNew)
	{
		SyncFlag(flags, m_flags, CVAR_ROM);
		SyncFlag(flags, m_flags, CVAR_CHEAT);

		SyncFlag(flags, m_flags, CVAR_SYSTEM);
		SyncFlag(flags, m_flags, CVAR_RENDERER);
		SyncFlag(flags, m_flags, CVAR_SOUND);
		SyncFlag(flags, m_flags, CVAR_GUI);
		SyncFlag(flags, m_flags, CVAR_GAME);
		SyncFlag(flags, m_flags, CVAR_TOOL);
	}

	SetModified();
}

void CVar::ClearFlag(const unsigned int flag)
{
	LOG_FUNC();

	::ClearFlag(m_flags, flag);

	SetModified();
}

void common::cvar::CVar::SetModified()
{
	LOG_FUNC();

	::SetFlag(m_flags, CVAR_MODIFIED);

	if (m_legacyPtr)
	{
		SyncLegacy(true);
	}
}

void common::cvar::CVar::ClearModified()
{
	LOG_FUNC();

	::ClearFlag(m_flags, CVAR_MODIFIED);

	if (m_legacyPtr)
	{
		SyncLegacy(true);
	}
}

void CVar::Reset()
{
	LOG_FUNC();

	Set(m_resetString);
}

void CVar::Unlatch(const bool force)
{
	LOG_FUNC();

	if ((m_flags & CVAR_LATCH) && !m_latchedString.empty())
	{
		if (Com_ServerState() && !force)
		{
			Printf("%s will be changed for next game.\n", m_name.c_str());
		}
		else
		{
			m_valueString = m_latchedString;
			m_latchedString.clear();

			if (m_legacyPtr)
			{
				SyncLegacy(true);
			}
		}
	}
}

cvar_t* common::cvar::CVar::Legacy()
{
	LOG_FUNC();

	if (!m_legacyPtr)
	{
		m_legacyPtr = std::make_shared<cvar_t>();
		SyncLegacy(true);
	}
	else
	{
		SyncLegacy(false);
	}

	return m_legacyPtr.get();
}
