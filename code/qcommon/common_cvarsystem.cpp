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

bool common::cvar::Cvar_InfoValidate(const sz::string& value)
{
	LOG_FUNC();

	if (value.find("\\"))
		return false;
	if (value.find("\""))
		return false;
	if (value.find(";"))
		return false;

	return true;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, bool defaultValue, unsigned int flags, const sz::string& description)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, int64_t defaultValue,
	unsigned int flags, const sz::string& description, int64_t valueMin, int64_t valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, uint64_t defaultValue,
	unsigned int flags, const sz::string& description, uint64_t valueMin, uint64_t valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, int defaultValue, unsigned int flags,
	const sz::string& description, int valueMin, int valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, unsigned int defaultValue,
	unsigned int flags, const sz::string& description, unsigned int valueMin, unsigned int valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, float defaultValue,
	unsigned int flags, const sz::string& description, float valueMin, float valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, double defaultValue,
	unsigned int flags, const sz::string& description, double valueMin, double valueMax)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultValue, flags, description, valueMin, valueMax);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Register(const sz::string& name, const sz::string& defaultString,
	unsigned int flags, const sz::string& description)
{
	LOG_FUNC();

	try
	{
		if (IsFlagSet(flags, CVAR_USERINFO) || IsFlagSet(flags, CVAR_SERVERINFO))
		{
			if (!Cvar_InfoValidate(name))
			{
				Error("Invalid info CVar name\n");
				return nullptr;
			}
		}

		if (!m_CVarMap.contains(name))
		{
			m_CVarMap[name] = std::make_shared<CVar>(name, defaultString, flags, description);
		}

		return m_CVarMap[name];
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Find(const sz::string& name)
{
	LOG_FUNC();

	if (m_CVarMap.contains(name))
	{
		return m_CVarMap[name];
	}

	return nullptr;
}

inline std::shared_ptr<CVar> common::cvar::CVarSystem::Find(const char* name)
{
	return Find(sz::string(name));
}

std::vector<std::shared_ptr<CVar>> common::cvar::CVarSystem::FindAll(const sz::string& searchRegex, unsigned int flags) const
{
	LOG_FUNC();

	std::vector<std::shared_ptr<CVar>> cvar_list;

	try
	{
		auto regex = boost::make_u32regex(searchRegex.c_str());

		for (auto& kvPair : m_CVarMap)
		{
			// String pattern match
			if (!searchRegex.empty() && !searchRegex.is_space() && searchRegex != "*" && searchRegex != "*.*")
			{
				auto u32str = icu::UnicodeString::fromUTF8(kvPair.first.c_str());
				if (!u32regex_search(u32str, regex))
				{
					continue;
				}
			}

			// Filter by flags
			if (!(kvPair.second->Flags() & flags))
			{
				continue;
			}

			cvar_list.push_back(kvPair.second);

			// Sort the list
			std::ranges::sort(cvar_list, [](const std::shared_ptr<CVar>& a, const std::shared_ptr<CVar>& b) {
				return a->Name().compare(b->Name()) >= 0;
				});
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return cvar_list;
}

void common::cvar::CVarSystem::ListToConsole(const sz::string& searchRegex, unsigned int flags) const
{
	LOG_FUNC();

	try
	{
		auto cvar_list = FindAll(searchRegex, flags);

		if (!cvar_list.empty())
		{
			//Output the list
			Printf("\n");
			Printf("\tFlags    Sys  Name             Type Value              Description\n");
			Printf("\t-----    ---  ----             ---- -----              -----------\n");

			for (const auto& cvar : cvar_list)
			{
				Printf("\t%s\n", cvar->InfoString().c_str());
			}

			Printf("\n%i CVars listed\n\n", cvar_list.size());
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}
