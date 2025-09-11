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

typedef struct file_s {
	std::shared_ptr<common::filesystem::File> ptr;
} file_t;

EXTERNC void FS_Init(void)
{
	if (common::fs != nullptr)
	{
		common::fs->Initialize();
	}
}

EXTERNC qboolean FS_Initialized(void)
{
	if (common::fs != nullptr)
	{
		return static_cast<qboolean>(common::fs->IsInitialized());
	}

	return false;
}

EXTERNC void FS_Shutdown(void)
{
	if (common::fs != nullptr)
	{
		common::fs->Shutdown();
	}
}

EXTERNC void FS_SetGamedir(const char* dir)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return;
			}

			if (strstr(dir, "..") || strstr(dir, "/")
				|| strstr(dir, "\\") || strstr(dir, ":"))
			{
				Com_Printf("Gamedir should be a single filename, not a path\n");
				return;
			}

			//
			// free up any current game dir info
			//
			common::fs->RemoveAllSearchPaths();

			//
			// flush all data, so it will be forced to reload
			//
			if (dedicated && !dedicated->value)
			{
				Cbuf_AddText("vid_restart\nsnd_restart\n");
			}

			if (!strcmp(dir, BASEDIRNAME) || (*dir == 0))
			{
				Cvar_FullSet("gamedir", "", CVAR_SERVERINFO | CVAR_NOSET);
				Cvar_FullSet("game", "", CVAR_LATCH | CVAR_SERVERINFO);
			}
			else
			{
				Cvar_FullSet("gamedir", dir, CVAR_SERVERINFO | CVAR_NOSET);

				if (common::filesystem::cvar_fsEnableDiscs &&
					common::filesystem::cvar_fsEnableDiscs->Bool() &&
					common::filesystem::cvar_fsCDDir &&
					!common::filesystem::cvar_fsCDDir->String().empty())
				{
					common::fs->AddSearchPath(boost::filesystem::path(common::filesystem::cvar_fsCDDir->String().c_str()).append(dir));
				}

				common::fs->AddSearchPath(common::fs->GetBasePath().append(dir));
			}

			common::fs->AddDefaultSearchPaths();
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

EXTERNC const char* FS_Gamedir(void)
{
	if (common::filesystem::cvar_fsSavePath != nullptr)
	{
		return common::filesystem::cvar_fsSavePath->String().c_str();
	}

	return "";
}

EXTERNC const char* FS_NextPath(const char* prevPath)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
			}

			auto searchPaths = common::fs->GetSearchPaths();

			if (!prevPath && (!searchPaths.empty()))
			{
				return searchPaths[0].lexically_normal().generic_string().c_str();
			}

			auto previousPath = boost::filesystem::path(prevPath).lexically_normal().generic_path();

			bool foundFlag = false;

			for (auto& searchPath : searchPaths)
			{
				if (foundFlag)
				{
					return searchPath.lexically_normal().generic_string().c_str();
				}

				if (searchPath.lexically_normal().generic_path().compare(previousPath) == 0)
				{
					foundFlag = true;
				}
			}

			// We didn't find anything or we are at the last record
			return nullptr;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

EXTERNC const char** FS_ListFiles(const char* path, const char* extension, size_t* numFiles, const enum fsListFlags mustHave, const enum fsListFlags cantHave)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return nullptr;
			}

			const auto searchPath = boost::filesystem::path(path).lexically_normal().generic_path();
			auto extension_str = sz::string(".*");

			if (extension)
			{
				extension_str = extension;
			}

			if (!extension_str.starts_with("."))
			{
				extension_str = sz::string(".") + extension_str;
			}

			const auto entryList = common::fs->GetFileList(searchPath.parent_path(), extension_str);

			std::vector<boost::filesystem::path> out_file_paths;

			for (auto& kvPair : entryList)
			{
				if (kvPair.second.isDirectory)
				{
					if (mustHave & FS_LIST_DIRECTORIES)
					{
						out_file_paths.push_back(kvPair.first);
						continue;
					}
					else if (cantHave & FS_LIST_DIRECTORIES)
					{
						continue;
					}
				}

				if (kvPair.second.isSymlink)
				{
					if (mustHave & FS_LIST_SYMLINKS)
					{
						out_file_paths.push_back(kvPair.first);
						continue;
					}
					else if (cantHave & FS_LIST_SYMLINKS)
					{
						continue;
					}
				}

				// File
				if (mustHave & FS_LIST_FILES)
				{
					out_file_paths.push_back(kvPair.first);
					continue;
				}
				else if (cantHave & FS_LIST_FILES)
				{
					continue;
				}

				// Otherwise, no flags
				out_file_paths.push_back(kvPair.first);
			}

			const auto list = static_cast<const char**>(Z_Malloc(sizeof(const char*) * out_file_paths.size()));

			if (list != nullptr)
			{
				for (size_t i = 0; i < out_file_paths.size(); i++)
				{
					list[i] = _strdup(out_file_paths.at(i).lexically_normal().generic_string().c_str());
				}
			}

			return list;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}


EXTERNC static void FS_FreeList(const char** list, const size_t nFiles)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return;
			}

			if (list == nullptr || nFiles == 0)
			{
				return;
			}

			for (size_t i = 0; i < nFiles; i++)
			{
				if (list[i])
				{
					free(static_cast<void*>(const_cast<char*>(list[i])));
					list[i] = nullptr;
				}
			}

			Z_Free(static_cast<void*>(list));
			list = nullptr;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

EXTERNC static bool FS_FileExists(const char* filePath)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return false;
			}

			return common::fs->FileExists(filePath);
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

EXTERNC void FS_ExecAutoexec(void)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
			}

			auto gameDirStr = sz::string(Cvar_VariableString("gamedir"));

			auto file_path = boost::filesystem::path("/");

			if (!gameDirStr.empty())
			{
				file_path.append(gameDirStr + "/");
			}
			else
			{
				file_path.append(sz::string(BASEDIRNAME) + "/");
			}

			file_path.append("autoexec.cfg");

			if (common::fs->FileExists(file_path))
			{
				Cbuf_AddText("exec autoexec.cfg\n");
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

EXTERNC size_t FS_FOpenFileRead(const char* fileName, file_t** file)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return 0;
			}

			if (common::fs->FileExists(fileName))
			{
				//if (!(*file))
				{
					(*file) = static_cast<file_t*>(Z_Malloc(sizeof(file_t)));
					(*file)->ptr = nullptr;
				}

				if (*file)
				{
					(*file)->ptr = common::fs->OpenFile(fileName, common::filesystem::FileMode::FS_READ);

					if (!((*file)->ptr))
					{
						LOG(error) << "Failed to open file '" << fileName << "' for reading.";

						FS_FCloseFile(*file);

						return 0;
					}

					return (*file)->ptr->Length();
				}
			}

			Com_DPrintf("FindFile: can't find %s\n", fileName);

			(*file) = nullptr;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;
}

EXTERNC file_t* FS_FOpenFileWrite(const char* fileName)
{
	LOG_FUNC();

	file_t* f = nullptr;

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return nullptr;
			}

			f = static_cast<file_t*>(Z_Malloc(sizeof(file_t)));
			f->ptr = nullptr;

			if (f)
			{
				(f)->ptr = common::fs->OpenFile(fileName, common::filesystem::FileMode::FS_WRITE);

				if (!(f->ptr))
				{
					LOG(error) << "Failed to open file '" << fileName << "' for writing.";

					FS_FCloseFile(f);

					return nullptr;
				}

				return f;
			}
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

EXTERNC void FS_FCloseFile(file_t* f)
{
	if (f && (common::fs != nullptr))
	{
		if (f->ptr)
		{
			common::fs->CloseFile(f->ptr);

			f->ptr = nullptr;
		}

		Z_Free(f);

		f = nullptr;
	}
}

void CDAudio_Stop(void);

EXTERNC size_t FS_LoadFile(const char* filePath, void** buffer)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return 0;
			}

			byte* allocated_buffer = nullptr;

			auto file = common::fs->OpenFile(filePath, common::filesystem::FileMode::FS_READ);

			if (!file)
			{
				LOG(error) << "Failed to open file '" << filePath << "' for reading.";

				if (buffer)
				{
					*buffer = nullptr;
				}

				return 0;
			}

			size_t fileLength = file->Length();

			if (!buffer)
			{
				common::fs->CloseFile(file);
				file = nullptr;
				return fileLength;
			}

			allocated_buffer = static_cast<byte*>(Z_Malloc(fileLength));
			*buffer = allocated_buffer;

			size_t bytesRead = 0;
			size_t bytesRemaining = fileLength;
			unsigned short tries = 0;

			while (bytesRemaining > 0)
			{
				bytesRead = file->Read(allocated_buffer, bytesRemaining);

				if (bytesRead == 0)
				{
					// we might have been trying to read from a CD
					if (!tries)
					{
						tries = 1;
						CDAudio_Stop();
					}
					else
					{
						return fileLength - bytesRemaining; // Com_Error(ERR_FATAL, "FS_Read: 0 bytes read");
					}
				}

				bytesRemaining -= bytesRead;
				allocated_buffer += bytesRead;
			}

			common::fs->CloseFile(file);
			file = nullptr;
			return fileLength;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;
}


EXTERNC size_t FS_Read(void* buffer, const size_t bufferSize, const file_t* f)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return 0;
			}

			byte* localBufferPtr = static_cast<byte*>(buffer);

			if (f && f->ptr)
			{
				// read in chunks
				size_t bytesRemaining = bufferSize;
				size_t totalBytesRead = 0;
				unsigned short tries = 0;

				while (bytesRemaining)
				{
					size_t block = bytesRemaining;
					size_t bytesRead = 0;
					block = std::min<size_t>(block, MAX_READ);

					bytesRead = f->ptr->Read(localBufferPtr, block);
					if (bytesRead == 0)
					{
						// we might have been trying to read from a CD
						if (!tries)
						{
							tries = 1;
							CDAudio_Stop();
						}
						else
						{
							return bytesRead - bytesRemaining; // Com_Error(ERR_FATAL, "FS_Read: 0 bytes read");
						}
					}
					else
					{
						totalBytesRead += bytesRead;
					}

					bytesRemaining -= bytesRead;
					localBufferPtr += bytesRead;
				}
				return totalBytesRead;
			}
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;
}


size_t FS_Write(const void* buffer, const size_t bufferSize, const file_t* f)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return 0;
			}

			if (f && f->ptr)
			{
				// write in chunks
				size_t bytesRemaining = bufferSize;
				size_t totalBytesWritten = 0;
				unsigned short tries = 0;

				const byte* localBufferPtr = static_cast<const byte*>(buffer);

				while (bytesRemaining)
				{
					size_t block = bytesRemaining;
					size_t bytesWritten = 0;
					block = std::min<size_t>(block, MAX_WRITE);

					bytesWritten = f->ptr->Write(localBufferPtr, block);

					if (bytesWritten == 0)
					{
						if (!tries)
						{
							tries = 1;
						}
						else
						{
							return totalBytesWritten; // Com_Printf("FS_Write: 0 bytes written\n");
						}
					}
					else
					{
						totalBytesWritten += bytesWritten;
					}

					bytesRemaining -= bytesWritten;
					localBufferPtr += bytesWritten;
				}

				f->ptr->Flush();

				return totalBytesWritten;
			}
		}

	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;
}

EXTERNC size_t FS_FileLength(const file_t* f)
{
	if (f && f->ptr)
	{
		return f->ptr->Length();
	}

	return 0;
}

EXTERNC void FS_FreeFile(void* buffer)
{
	if (buffer)
	{
		Z_Free(buffer);
	}
}

EXTERNC void FS_CreatePath(const char* path)
{
	if (path && (path[0] != '\0') && (common::fs != nullptr))
	{
		if (!common::fs->IsInitialized())
		{
			Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
		}

		auto _ = common::fs->MakeDirectory(path);
	}
}

EXTERNC int	Developer_searchpath(const int who)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Error(ERR_FATAL, "Filesystem call made without initialization\n");
				return 0;
			}

			auto searchPaths = common::fs->GetSearchPaths();

			for (auto& searchPath : searchPaths)
			{
				if (sz::string(searchPath.lexically_normal().generic_string()).contains("xatrix"))
				{
					return 1;
				}
				else if (sz::string(searchPath.lexically_normal().generic_string()).contains("rogue"))
				{
					return 2;
				}
			}
		}

	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;

}

EXTERNC void FS_Path_f(void)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Printf("Filesystem call made without initialization\n");
				return;
			}

			Com_Printf("Current search path:\n");

			auto searchPaths = common::fs->GetSearchPaths();

			for (auto& searchPath : searchPaths)
			{
				Com_Printf("\t%s\n", searchPath.generic_string().c_str());
			}

			auto fileLinks = common::fs->GetFileLinks();

			if (!fileLinks.empty())
			{
				Com_Printf("\nLinks:\n");

				for (auto& kvPair : fileLinks)
				{
					Com_Printf("%s -> %s\n", kvPair.first.generic_string().c_str(), kvPair.second.generic_string().c_str());
				}
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

EXTERNC void FS_Link_f(void)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Printf("Filesystem call made without initialization\n");
				return;
			}

			if (Cmd_Argc() != 3)
			{
				Com_Printf("USAGE: link <from> <to>\n");
				return;
			}

			if (!common::fs->MakeFileLink(Cmd_Argv(1), Cmd_Argv(2)))
			{
				Com_Printf("Failed to make link.\n");
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

static size_t OutputDirectoryContentsToConsole(const boost::filesystem::path& path, sz::string& extension, bool recurse = false, bool printHeader = true)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Printf("Filesystem call made without initialization\n");
				return 0;
			}

			auto entryList = common::fs->GetFileList(path, extension);
			size_t entryCount = entryList.size();

			if (printHeader)
			{
				Com_Printf("\nListing of %s/*%s\n\n", path.generic_string().c_str(), extension.c_str());

				auto headerStr = Format("{:4} {:20} {:12} {}", "Mode", "Last Modified", "Size", "Name");
				auto headerBarStr = Format("{:4} {:20} {:12} {}", "----", "-------------", "----", "----");

				Com_Printf("\t%s\n", headerStr.c_str());
				Com_Printf("\t%s\n", headerBarStr.c_str());
			}

			for (auto& kvPair : entryList)
			{
				auto attribs = sz::string();
				auto fileSize = sz::string();
				auto lastModified = sz::string();

				if (kvPair.second.isDirectory)
				{
					attribs += "d";
					fileSize = FormatBytes(kvPair.second.fileSize);
				}
				else if (kvPair.second.isSymlink)
				{
					attribs += "l";
				}
				else
				{
					attribs += "-";
				}

				if (kvPair.second.readOnly)
				{
					attribs += "r";
				}
				else
				{
					attribs += "-";
				}

				if (kvPair.second.lastModified > 0)
				{
					lastModified = time_t_to_string(kvPair.second.lastModified);
				}

				auto outStr = Format("{:4} {:20} {:12} {}", attribs, lastModified, fileSize, kvPair.first.generic_string());

				Com_Printf("\t%s\n", outStr.c_str());

				if ((kvPair.second.isDirectory || kvPair.second.isSymlink) && recurse)
				{
					entryCount += OutputDirectoryContentsToConsole(kvPair.first, extension, true, false);
				}
			}

			return entryCount;
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return 0;
}

EXTERNC void FS_Dir_f(void)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Printf("Filesystem call made without initialization\n");
				return;
			}

			auto argCount = Cmd_Argc();
			if (argCount < 2 || argCount > 3)
			{
				Com_Printf("USAGE: dir <directory> [extension]\n");
				return;
			}

			auto path = boost::filesystem::path(Cmd_Argv(1));
			auto extension = sz::string(".*");

			if (argCount == 3)
			{
				extension = sz::string(Cmd_Argv(1));

				if (!extension.starts_with("."))
				{
					extension = sz::string(".") + extension;
				}
			}

			size_t entryCount = OutputDirectoryContentsToConsole(path, extension);

			Com_Printf("\n\n%d records\n", entryCount);
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

EXTERNC void FS_DirTree_f(void)
{
	LOG_FUNC();

	try
	{
		if (common::fs != nullptr)
		{
			if (!common::fs->IsInitialized())
			{
				Com_Printf("Filesystem call made without initialization\n");
				return;
			}

			auto argCount = Cmd_Argc();
			if (argCount < 2 || argCount > 3)
			{
				Com_Printf("USAGE: dirtree <directory> [extension]\n");
				return;
			}

			auto path = boost::filesystem::path(Cmd_Argv(1));
			auto extension = sz::string(".*");

			if (argCount == 3)
			{
				extension = sz::string(Cmd_Argv(1));

				if (!extension.starts_with("."))
				{
					extension = sz::string(".") + extension;
				}
			}

			size_t entryCount = OutputDirectoryContentsToConsole(path, extension, true);

			Com_Printf("\n\n%d records\n", entryCount);
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}