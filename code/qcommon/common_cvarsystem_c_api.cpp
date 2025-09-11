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

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

qboolean userinfo_modified = false;

/*
=====================
Cvar_CheckRange
=====================
*/
EXTERNC void Cvar_CheckRange(cvar_t* var, float min, float max, qboolean integral)
{
	var->validate = qtrue;
	var->min = min;
	var->max = max;
	var->integral = integral;

	// Force an initial range check
	Cvar_Set(var->name, var->string);
}

/*
============
Cvar_Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
EXTERNC cvar_t* Cvar_Get(const char* var_name, const char* var_value, const unsigned int flags)
{
	if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if (!common::cvar::Cvar_InfoValidate(var_name))
		{
			Com_Printf("Invalid info CVar name\n");
			return nullptr;
		}

		if (!common::cvar::Cvar_InfoValidate(var_value))
		{
			Com_Printf("Invalid info CVar value\n");
			return nullptr;
		}
	}

	if (auto var = common::cv->Find(var_name))
	{
		var->SetFlags(flags);

		return var->Legacy();
	}
	else
	{
		if (!var_value)
		{
			return nullptr;
		}

		var = common::cv->Register(var_name, var_value, flags, "Legacy CVar");

		if (var)
		{
			return var->Legacy();
		}
	}

	return nullptr;
}

EXTERNC inline cvar_t* Cvar_Set(const char* var_name, const char* value)
{
	if (auto var = common::cv->Find(var_name))
	{
		var->Set(value);

		if (var->Flags() & CVAR_USERINFO)
		{
			userinfo_modified = true;	// transmit at next opportunity
		}

		return var->Legacy();
	}

	return Cvar_Get(var_name, value, 0);
}

EXTERNC inline cvar_t* Cvar_ForceSet(const char* var_name, const char* value)
{
	if (auto var = common::cv->Find(var_name))
	{
		var->Set(value, true);

		if (var->Flags() & CVAR_USERINFO)
		{
			userinfo_modified = true;	// transmit at next opportunity
		}

		return var->Legacy();
	}

	return Cvar_Get(var_name, value, 0);
}

EXTERNC inline cvar_t* Cvar_FullSet(const char* var_name, const char* value, const unsigned int flags)
{
	if (const auto var = common::cv->Find(var_name))
	{
		var->Set(value, true);

		if (var->Flags() & CVAR_USERINFO)
		{
			userinfo_modified = true;	// transmit at next opportunity
		}

		return var->Legacy();
	}

	return Cvar_Get(var_name, value, flags);
}

EXTERNC inline void Cvar_SetValue(const char* var_name, const float value)
{
	if (const auto var = common::cv->Find(var_name))
	{
		var->Set(value);
	}
}

EXTERNC float Cvar_VariableValue(const char* var_name)
{
	const auto var = common::cv->Find(var_name);

	if (var != nullptr)
	{
		return var->Float();
	}

	return 0.0f;
}

EXTERNC const char* Cvar_VariableString(const char* var_name)
{
	if (const auto var = common::cv->Find(var_name); var != nullptr)
	{
		return var->String().c_str();
	}

	return "";
}

EXTERNC const char* Cvar_CompleteVariable(const char* partial)
{
	auto cvar_list = common::cv->FindAll(partial, 0);

	// List already sorted alphabetically, sort so shortest names are first
	std::ranges::sort(cvar_list, [](const std::shared_ptr<CVar>& a, const std::shared_ptr<CVar>& b) {
		return a->Name().length() < b->Name().length();
		});

	for (const auto& cvar : cvar_list)
	{
		if (cvar->Name().starts_with(partial))
		{
			// Return the first cvar that begins with the search string, which should be the shortest
			return cvar->Name().c_str();
		}
	}
}

EXTERNC void Cvar_GetLatchedVars(void)
{
	const auto cvar_list = common::cv->FindAll("*", CVAR_LATCH);

	for (const auto& cvar : cvar_list)
	{
		cvar->Unlatch(true);
	}
}

EXTERNC qboolean Cvar_Command(void)
{
	if (auto cvar = common::cv->Find(Cmd_Argv(0)))
	{
		// perform a variable print or set
		if (Cmd_Argc() == 1)
		{
			Com_Printf("\t%s\n", cvar->InfoString().c_str());

			return true;
		}

		cvar->Set(Cmd_Argv(1));

		return true;
	}
	else
	{
		return false;
	}

	return false;
}

EXTERNC const char* Cvar_BitInfo(const unsigned int bit)
{
	static char	info[MAX_INFO_STRING];
	sz::memset(&info, 0, MAX_INFO_STRING);

	const auto cvar_list = common::cv->FindAll("*", bit);

	for (const auto& cvar : cvar_list)
	{
		Info_SetValueForKey(info, cvar->Name().c_str(), cvar->String().c_str());
	}

	return info;
}

EXTERNC void Cvar_WriteVariables(const char* path)
{
	const auto cvar_list = common::cv->FindAll("*", CVAR_ARCHIVE);

	if (!cvar_list.empty())
	{
		auto file = common::fs->OpenFile(path, common::filesystem::FileMode::FS_APPEND);

		if (file)
		{
			for (auto& cvar : cvar_list)
			{
				file->WriteString(Format("set %s \"%s\"\n", cvar->Name(), cvar->String()));
			}

			common::fs->CloseFile(file);
		}
	}
}

EXTERNC void Cvar_Init(void)
{
	common::cv->Initialize();
}

// returns an info string containing all the CVAR_USERINFO cvars
EXTERNC const char* Cvar_Userinfo(void)
{
	return Cvar_BitInfo(CVAR_USERINFO);
}

// returns an info string containing all the CVAR_SERVERINFO cvars
EXTERNC const char* Cvar_Serverinfo(void)
{
	return Cvar_BitInfo(CVAR_SERVERINFO);
}

EXTERNC void Cvar_Toggle_f(void)
{
	auto	token_count = Cmd_Argc();

	if (token_count < 2 || token_count > 3)
	{
		Com_Printf("USAGE:\n"
			"\ttoggle <variable>  - toggles between '0'/'1' (if numeric) or 'false'/'true' (if boolean)\n"
			"\ttoggle <variable> <value> - toggles between '0' and <value>\n");
		return;
	}

	if (const auto var = common::cv->Find(Cmd_Argv(1)); var != nullptr)
	{
		auto original_value_str = var->String();

		// Bool
		switch (var->Type())
		{
		case common::cvar::CVAR_TYPE_BOOL:
			if (var->Bool())
			{
				var->Set(false);
			}
			else
			{
				var->Set(true);
			}
			break;
		case common::cvar::CVAR_TYPE_INT64:
		case common::cvar::CVAR_TYPE_INT:
			if (var->Int64() != 0)
			{
				var->Set(0);
			}
			else // (current_value == 0)
			{
				int64_t set_value = 0;

				if (token_count == 3)
				{
					set_value = boost::lexical_cast<int64_t>(Cmd_Argv(2));
				}

				var->Set(set_value);
			}
			break;
		case common::cvar::CVAR_TYPE_UINT64:
		case common::cvar::CVAR_TYPE_UINT:
			if (var->UInt64() != 0)
			{
				var->Set(0);
			}
			else // (current_value == 0)
			{
				uint64_t set_value = 0;

				if (token_count == 3)
				{
					set_value = boost::lexical_cast<uint64_t>(Cmd_Argv(2));
				}

				var->Set(set_value);
			}
			break;
		case common::cvar::CVAR_TYPE_DOUBLE:
		case common::cvar::CVAR_TYPE_FLOAT:
			if (var->Double() != 0.0)
			{
				var->Set(0);
			}
			else // (current_value == 0)
			{
				double set_value = 0.0;

				if (token_count == 3)
				{
					set_value = boost::lexical_cast<double>(Cmd_Argv(2));
				}

				var->Set(set_value);
			}
			break;
		case common::cvar::CVAR_TYPE_STRING:
		default:
			break;
		}
		
		Com_Printf("Set CVar %s = '%s' (originally '%s')\n", var->Name().c_str(), var->String().c_str(), original_value_str.c_str());
	}
	else
	{
		Com_Printf("WARNING: CVar \"%s\" not found", Cmd_Argv(1));
	}
}

EXTERNC void Cvar_Set_f(void)
{
	auto	token_count = Cmd_Argc();
	unsigned int		flags = 0;

	if (token_count != 3 && token_count != 4)
	{
		Com_Printf("USAGE: set <variable> <value> [u / s]\n");
		return;
	}

	if (token_count == 4)
	{
		auto flag_string = sz::string(Cmd_Argv(3));

		if (flag_string == "u")
		{
			flags = CVAR_USERINFO;
		}
		else if (flag_string == "s")
		{
			flags = CVAR_SERVERINFO;
		}
		else
		{
			Com_Printf("flags can only be 'u' or 's'\n");
			return;
		}
		Cvar_FullSet(Cmd_Argv(1), Cmd_Argv(2), flags);
	}
	else
	{
		Cvar_Set(Cmd_Argv(1), Cmd_Argv(2));
	}
}

EXTERNC void Cvar_Reset_f(void)
{
	auto	token_count = Cmd_Argc();

	if (token_count != 2)
	{
		Com_Printf("USAGE: reset <variable>\n");
		return;
	}

	if (const auto var = common::cv->Find(Cmd_Argv(1)); var != nullptr)
	{
		var->Reset();
	}
	else
	{
		Com_Printf("WARNING: CVar \"%s\" not found", Cmd_Argv(1));
	}
}

EXTERNC void Cvar_List_f(void)
{
	auto	token_count = Cmd_Argc();

	if (token_count != 1 && token_count != 2)
	{
		Com_Printf("USAGE: cvarlist [search regex]\n");
		return;
	}

	if (token_count == 2)
	{
		common::cv->ListToConsole(Cmd_Argv(1));
	}
	else
	{
		common::cv->ListToConsole("*");
	}
}