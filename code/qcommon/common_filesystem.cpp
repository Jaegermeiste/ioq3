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

/*
==============
Sys_EXEPath
==============
*/
static boost::filesystem::path Sys_EXEPath()
{
	try
	{
		char exe[MAX_OSPATH] = { 0 };
		GetModuleFileNameA(nullptr, exe, sizeof(exe) - 1);

		boost::filesystem::path path(exe);
		return path.lexically_normal().generic_path();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}

static boost::filesystem::path Sys_DefaultBasePath()
{
	LOG_FUNC();

	try {
		boost::filesystem::path path = Sys_EXEPath().parent_path();

		if (!boost::filesystem::exists(path))
		{
			LOG(error) << "Base path does not exist: " << path.generic_string();

			// Try Current Working Directory
			path = boost::filesystem::current_path();

			if (!boost::filesystem::exists(path))
			{
				LOG(error) << "Base path does not exist: " << path.generic_string();
			}
		}

		return path.lexically_normal().generic_path();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}

/*
==============
Sys_DefaultSavePath
==============
*/
static boost::filesystem::path Sys_DefaultSavePath()
{
	LOG_FUNC();

	try
	{
		boost::filesystem::path path;
		PWSTR wpath = nullptr;

		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_SavedGames, KF_FLAG_CREATE | KF_FLAG_INIT, NULL, &wpath)))
		{
			if (wpath != nullptr)
			{
				path = wpath;

				CoTaskMemFree(wpath);
			}
			else
			{
				LOG(error) << "Failed to get 'Saved Games' known folder path.";
			}
		}

		if (path.empty() || !boost::filesystem::exists(path))
		{
			wpath = nullptr;

			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE | KF_FLAG_INIT, NULL, &wpath)))
			{
				if (wpath != nullptr)
				{
					path = wpath;

					CoTaskMemFree(wpath);
				}
				else
				{
					LOG(error) << "Failed to get 'Documents' known folder path.";
				}
			}
		}

		if (path.empty() || !boost::filesystem::exists(path))
		{
			path = Sys_EXEPath();
		}

		path.append("/id Software/Quake 2/");

		return path.lexically_normal().generic_path();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}


bool common::filesystem::FileSystem::Initialize()
{
	LOG_FUNC();

	try
	{
		// Check if already initialized
		if (m_initialized)
		{
			return true;
		}

		Printf("------ Initializing File System ------\n");

		if (PHYSFS_isInit())
		{
			Error("PhysFS is already initialized.");
		}
		else if (!PHYSFS_init("\0"))
		{
			Error("Failed to initialize PhysFS: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return false;
		}
		else
		{
			PHYSFS_Version compiled = {};
			PHYSFS_Version linked = {};
			PHYSFS_VERSION(&compiled);
			PHYSFS_getLinkedVersion(&linked);
			LOG(info) << "Compiled against PhysFS version " << compiled.major << "." << compiled.minor << "." << compiled.patch;
			LOG(info) << "Linked against PhysFS version " << linked.major << "." << linked.minor << "." << linked.patch;

			Printf("\tPhysFS Version: %d.%d.%d\n", linked.major, linked.minor, linked.patch);
		}

		if (!cvar_fsSavePath)
		{
			cvar_fsSavePath = common::cv->Register("fs_savePath", "", CVAR_NOSET | CVAR_ARCHIVE | CVAR_SYSTEM, "Safe writable directory.");

			// Check if fs_savePath is set to a valid path
			if (!cvar_fsSavePath)
			{
				LOG(error) << "Failed to get fs_savePath cvar.";
				return false;
			}

			// Get the base path from the CVar, or set a default if not set
			if (cvar_fsSavePath->String().empty())
			{
				m_savePath = Sys_DefaultSavePath();
			}
			else
			{
				m_savePath = cvar_fsSavePath->String().c_str();
			}

			if (!boost::filesystem::exists(m_savePath))
			{
				LOG(error) << "Save path does not exist: " << m_savePath.lexically_normal().generic_string();
				return false;
			}

			cvar_fsSavePath->Set(m_savePath.lexically_normal().generic_string());
			cvar_fsSavePath->ClearModified();

			if (!PHYSFS_setWriteDir(m_savePath.lexically_normal().generic_string().c_str()))
			{
				LOG(error) << "Failed to set write directory: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				return false;
			}
			else
			{
				LOG(info) << "Write directory set to: " << m_savePath.lexically_normal().generic_string();
			}
		}

		if (!cvar_fsBasedir)
		{
			cvar_fsBasedir = common::cv->Register("fs_basedir", "", CVAR_ROM | CVAR_SYSTEM, "Base executable directory.");

			// Check if fs_basePath is set to a valid path
			if (!cvar_fsBasedir)
			{
				LOG(error) << "Failed to get fs_basePath cvar.";
				return false;
			}

			// Get the base path from the CVar, or set a default if not set
			if (cvar_fsBasedir->String().empty())
			{
				m_basePath = Sys_DefaultBasePath();
			}
			else
			{
				m_basePath = cvar_fsBasedir->String().c_str();
			}

			if (!boost::filesystem::exists(m_basePath))
			{
				LOG(error) << "Base path does not exist: " << m_basePath.lexically_normal().generic_string();

				return false;
			}

			cvar_fsBasedir->Set(m_basePath.lexically_normal().generic_string());
			cvar_fsBasedir->ClearModified();
		}

		if (!cvar_fsEnableDiscs)
		{
			cvar_fsEnableDiscs = common::cv->Register("fs_enableDiscs", "1", CVAR_ARCHIVE | CVAR_SYSTEM, "Enable reading from disc media.");

			if (!cvar_fsEnableDiscs)
			{
				LOG(error) << "Failed to get fs_enableDiscs cvar.";
				return false;
			}

			if (!cvar_fsEnableDiscs->Bool())
			{
				LOG(info) << "Disc media support is disabled.";
			}
			else
			{
				LOG(info) << "Disc media support is enabled.";
			}
		}

		if (!cvar_fsCDDir)
		{
			cvar_fsCDDir = common::cv->Register("cddir", "", CVAR_ROM | CVAR_SYSTEM, "Disc drive with Quake 2 media.");

			if (!cvar_fsCDDir)
			{
				LOG(error) << "Failed to get cddir cvar.";
				return false;
			}
		}

		if (cvar_fsEnableDiscs->Bool())
		{
			auto diskDrives = PHYSFS_getCdRomDirs();

			if (diskDrives == nullptr)
			{
				LOG(warning) << "Failed to get disc media directories: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			}
			else
			{
				for (auto i = diskDrives; *i != nullptr; i++)
				{
					// where activision put the stuff...
					const auto discRootDir = boost::filesystem::path(*i).append("install/data/").lexically_normal().generic_path();
					const auto testFile = (discRootDir / boost::filesystem::path("/quake2.exe")).lexically_normal().generic_path();

					if (boost::filesystem::exists(discRootDir) &&
						boost::filesystem::is_directory(discRootDir) &&
						boost::filesystem::exists(testFile) &&
						boost::filesystem::is_regular_file(testFile))
					{
						LOG(info) << "Found disc media: " << discRootDir.root_path().lexically_normal().generic_string();

						cvar_fsCDDir->Set(discRootDir.lexically_normal().generic_string());

						break; // Don't add further discs
					}
				}
				PHYSFS_freeList(static_cast<void*>(diskDrives));
			}
		}

		if (!cvar_fsBufferSizeBytes)
		{
			cvar_fsBufferSizeBytes = common::cv->Register("fs_bufferSizeBytes", 65536ul,  CVAR_ARCHIVE | CVAR_SYSTEM, "Buffer size in Bytes for reading/writing from the filesystem.", 0, 1048576);

			if (!cvar_fsBufferSizeBytes)
			{
				LOG(error) << "Failed to get fs_bufferSizeBytes cvar.";
				return false;
			}
		}

		AddDefaultSearchPaths();

		Cmd_AddCommand("path", FS_Path_f);
		Cmd_AddCommand("link", FS_Link_f);
		Cmd_AddCommand("dir", FS_Dir_f);
		Cmd_AddCommand("dirtree", FS_DirTree_f);

		m_initialized = true;

		Printf("\tFile System Initialized.\n");
		Printf("--------------------------------------\n");

		return m_initialized;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

void common::filesystem::FileSystem::Shutdown()
{
	LOG_FUNC();

	try
	{
		if (!m_initialized)
		{
			return;
		}

		RemoveAllSearchPaths();

		if (!PHYSFS_deinit())
		{
			LOG(error) << "Failed to deinitialize PhysFS: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		}

		m_fileLinks.clear();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	m_initialized = false;

	Printf("File System Shutdown.\n");
}

bool common::filesystem::FileSystem::AddSearchPath(const boost::filesystem::path& path, const bool append)
{
	LOG_FUNC();

	try
	{
		if (path.empty())
		{
			LOG(error) << "Search path is empty.";
			return false;
		}

		if (!boost::filesystem::exists(path))
		{
			LOG(error) << "Search path does not exist: " << path.generic_string();
			return false;
		}

		if ((path.extension() == ".pk3") ||
			(path.extension() == ".pk4") ||
			(path.extension() == ".pkx") ||
			(path.extension() == ".pkz") ||
			(path.extension() == ".zip") ||
			(path.extension() == ".pak"))
		{
			zipFiles.push_back(*i);
		}

		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return false;
		}

		if (!PHYSFS_mount(path.lexically_normal().generic_string().c_str(), nullptr, append ? TRUE : FALSE))
		{
			LOG(error) << "Failed to mount path: " << path.generic_string() << " - " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return false;
		}

		return true;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

inline bool common::filesystem::FileSystem::AddSearchPath(const sz::string& path, const bool append)
{
	return AddSearchPath(boost::filesystem::path(path.c_str()), append);
}

inline bool common::filesystem::FileSystem::AddSearchPath(const char* path, const bool append)
{
	return AddSearchPath(boost::filesystem::path(path), append);
}

bool common::filesystem::FileSystem::AddDefaultSearchPaths() const
{
	LOG_FUNC();

	try
	{
		// Add the save path
		if (!AddSearchPath(m_savePath))
		{
			LOG(error) << "Failed to add save search path: " << m_savePath.generic_string();
			return false;
		}
		else
		{
			LOG(info) << "Added save search path: " << m_savePath.generic_string();
		}

		// Add the save overrides path if it exists
		auto saveOverridesDir = boost::filesystem::path(m_savePath).append("/overrides/").lexically_normal().generic_path();
		if (boost::filesystem::exists(saveOverridesDir) && boost::filesystem::is_directory(saveOverridesDir))
		{
			if (!AddSearchPath(saveOverridesDir))
			{
				LOG(error) << "Failed to add save overrides search path: " << saveOverridesDir.generic_string();
				return false;
			}
			else
			{
				LOG(info) << "Added save overrides search path: " << saveOverridesDir.generic_string();
			}
		}

		// Add the base path
		if (!AddSearchPath(m_basePath))
		{
			LOG(error) << "Failed to add base search path: " << m_basePath.generic_string();
			return false;
		}
		else
		{
			LOG(info) << "Added base search path: " << m_basePath.generic_string();
		}

		// Add the install overrides path if it exists
		auto installOverridesDir = boost::filesystem::path(m_basePath).append("/overrides/").lexically_normal().generic_path();
		if (boost::filesystem::exists(installOverridesDir) && boost::filesystem::is_directory(installOverridesDir))
		{
			if (!AddSearchPath(installOverridesDir))
			{
				LOG(error) << "Failed to add install overrides search path: " << installOverridesDir.generic_string();
				return false;
			}
			else
			{
				LOG(info) << "Added install overrides search path: " << installOverridesDir.generic_string();
			}
		}

		// Add the default gamedir path
		auto defaultGameDir = boost::filesystem::path(m_basePath).append(std::string("/") + BASEDIRNAME + "/").lexically_normal().generic_path();
		if (!AddSearchPath(defaultGameDir))
		{
			LOG(error) << "Failed to add default gamedir search path: " << defaultGameDir.generic_string();
			return false;
		}
		else
		{
			LOG(info) << "Added default gamedir search path: " << defaultGameDir.generic_string();
		}

		// Add disc drive if appropriate
		if (cvar_fsEnableDiscs && cvar_fsEnableDiscs->Bool())
		{
			if (cvar_fsCDDir && (!cvar_fsCDDir->String().empty()))
			{
				// where activision put the stuff...
				const auto discRootDir = boost::filesystem::path(cvar_fsCDDir->String().c_str()).lexically_normal().generic_path();
				const auto discDataDir = (discRootDir / boost::filesystem::path(BASEDIRNAME)).lexically_normal().generic_path();

				if (boost::filesystem::exists(discRootDir) &&
					boost::filesystem::is_directory(discRootDir) &&
					boost::filesystem::exists(discDataDir) &&
					boost::filesystem::is_directory(discDataDir))
				{
					if (!AddSearchPath(discDataDir))
					{
						LOG(error) << "Failed to add disc media search path: " << discDataDir.lexically_normal().generic_string();
						return false;
					}
					else
					{
						LOG(info) << "Added disc media search path: " << discDataDir.lexically_normal().generic_string();
					}
				}
			}
		}

		// Find packfiles
		auto fileListing = PHYSFS_enumerateFiles("/");
		if (fileListing == nullptr)
		{
			LOG(error) << "Failed to get file listing: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return false;
		}

		std::vector<sz::string> zipFiles;
		std::vector<sz::string> packFiles;

		for (auto i = fileListing; *i != nullptr; i++)
		{
			auto filePath = boost::filesystem::path(*i);

			if ((filePath.extension() == ".pk3") ||
				(filePath.extension() == ".pk4") ||
				(filePath.extension() == ".pkx") ||
				(filePath.extension() == ".pkz") ||
				(filePath.extension() == ".zip"))
			{
				zipFiles.push_back(*i);
			}
			else if (filePath.extension() == ".pak")
			{
				packFiles.push_back(*i);
			}
		}
		PHYSFS_freeList(static_cast<void*>(fileListing));

		std::ranges::reverse(zipFiles);
		std::ranges::reverse(packFiles);

		// Add zip packfiles first
		for (auto&& file : zipFiles)
		{
			if (!AddSearchPath(file))
			{
				LOG(error) << "Failed to add .zip search path: " << file;
				return false;
			}
			else
			{
				LOG(info) << "Added search path: " << file;
			}
		}

		// Add pak packfiles last
		for (auto&& file : packFiles)
		{
			if (!AddSearchPath(file))
			{
				LOG(error) << "Failed to add .pak search path: " << file;
				return false;
			}
			else
			{
				LOG(info) << "Added search path: " << file;
			}
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

bool common::filesystem::FileSystem::RemoveSearchPath(const boost::filesystem::path& path)
{
	LOG_FUNC();

	try
	{
		if (path.empty())
		{
			LOG(error) << "Search path is empty.";
			return false;
		}

		if (!boost::filesystem::exists(path))
		{
			LOG(error) << "Search path does not exist: " << path.generic_string();
			return false;
		}

		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return false;
		}

		if (!PHYSFS_unmount(path.lexically_normal().generic_string().c_str()))
		{
			LOG(error) << "Failed to unmount path: " << path.generic_string() << " - " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return false;
		}

		return true;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

bool common::filesystem::FileSystem::RemoveSearchPath(const sz::string& path)
{
	return RemoveSearchPath(boost::filesystem::path(path.c_str()));
}

bool common::filesystem::FileSystem::RemoveSearchPath(const char* path)
{
	return RemoveSearchPath(boost::filesystem::path(path));
}

void common::filesystem::FileSystem::RemoveAllSearchPaths(void) const
{
	LOG_FUNC();

	try
	{
		auto searchPaths = GetSearchPaths();

		for (auto& path : searchPaths)
		{
			if (!RemoveSearchPath(path))
			{
				LOG(warning) << "Failed to remove search path '" << path.lexically_normal().generic_string() << "'!";
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

std::vector<boost::filesystem::path> common::filesystem::FileSystem::GetSearchPaths() const
{
	LOG_FUNC();

	try
	{
		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return {};
		}

		std::vector<boost::filesystem::path> searchPaths;

		char** list_items = nullptr;
		for (list_items = PHYSFS_getSearchPath(); *list_items != nullptr; list_items++)
		{
			searchPaths.push_back(boost::filesystem::path(*list_items).lexically_normal().generic_path());
		}
		PHYSFS_freeList(static_cast<void*>(list_items));
		list_items = nullptr;

		return searchPaths;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}

bool common::filesystem::FileSystem::MakeDirectory(const boost::filesystem::path& path) const
{
	LOG_FUNC();

	try
	{
		if (path.empty())
		{
			LOG(error) << "Path is empty.";
			return false;
		}

		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return false;
		}

		if (!PHYSFS_mkdir(path.lexically_normal().parent_path().generic_string().c_str()))
		{
			LOG(warning) << "Failed to create path '" << path.lexically_normal().parent_path().generic_string() << "': " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		}

		return true;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

inline bool common::filesystem::FileSystem::MakeDirectory(const sz::string& filePath) const
{
	return MakeDirectory(boost::filesystem::path(filePath.c_str()));
}

inline bool common::filesystem::FileSystem::MakeDirectory(const char* filePath) const
{
	return MakeDirectory(boost::filesystem::path(filePath));
}

bool common::filesystem::FileSystem::FileExists(const boost::filesystem::path& filePath) const
{
	LOG_FUNC();

	try
	{
		if (filePath.empty())
		{
			LOG(error) << "File path is empty.";
			return false;
		}

		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return false;
		}

		return static_cast<bool>(PHYSFS_exists(filePath.generic_string().c_str()));
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

inline bool common::filesystem::FileSystem::FileExists(const sz::string& filePath) const
{
	auto path = boost::filesystem::path(filePath.c_str());
	return FileExists(path);
}

inline bool common::filesystem::FileSystem::FileExists(const char* filePath) const
{
	auto path = boost::filesystem::path(filePath);
	return FileExists(path);
}

bool common::filesystem::FileSystem::MakeFileLink(const boost::filesystem::path& from, const boost::filesystem::path& to)
{
	LOG_FUNC();

	try
	{
		if (from.empty())
		{
			LOG(error) << "From file path is empty.";
			return false;
		}

		if (to.empty())
		{
			LOG(error) << "To file path is empty.";
			return false;
		}

		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return false;
		}

		auto normalFrom = from.lexically_normal().generic_path();
		auto normalTo = to.lexically_normal().generic_path();

		// Check if destination exists (OK if source is virtual)
		if (FileExists(normalTo))
		{
			// Simply overwrite if exists already
			if (m_fileLinks.contains(normalFrom))
			{
				LOG(warning) << "Overwriting file link for '" << normalFrom.generic_string() << "': '" << m_fileLinks[normalFrom].generic_string() << "' -> '" << normalTo.generic_string() << "'.";
			}

			m_fileLinks[normalFrom] = normalTo;

			return true;
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

inline bool common::filesystem::FileSystem::MakeFileLink(const sz::string& from, const sz::string& to)
{
	return MakeFileLink(boost::filesystem::path(from.c_str()), boost::filesystem::path(to.c_str()));
}

inline bool common::filesystem::FileSystem::MakeFileLink(const char* from, const char* to)
{
	return MakeFileLink(boost::filesystem::path(from), boost::filesystem::path(to));
}

bool common::filesystem::FileSystem::RemoveFileLink(const boost::filesystem::path& from)
{
	LOG_FUNC();

	try
	{
		if (from.empty())
		{
			LOG(error) << "From file path is empty.";
			return false;
		}

		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return false;
		}

		auto normalFrom = from.lexically_normal().generic_path();

		if (m_fileLinks.contains(normalFrom))
		{
			LOG(info) << "Removing file link for '" << normalFrom.generic_string() << "': '" << m_fileLinks[normalFrom].generic_string() << "'.";

			m_fileLinks.erase(normalFrom);

			return true;
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

std::unordered_map<boost::filesystem::path, boost::filesystem::path> common::filesystem::FileSystem::GetFileLinks()
{
	LOG_FUNC();

	try
	{
		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return {};
		}

		return m_fileLinks;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}

std::map<boost::filesystem::path, common::filesystem::FileProperties> common::filesystem::FileSystem::GetFileList(const boost::filesystem::path& path,
                                                                                                                  const std::set<sz::string>& extensions)
{
	LOG_FUNC();

	try
	{
		auto fileListing = PHYSFS_enumerateFiles(path.generic_string().c_str());

		if (fileListing == nullptr)
		{
			LOG(error) << "Failed to get file listing: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return {};
		}

		std::map<boost::filesystem::path, FileProperties> fileList;

		for (auto i = fileListing; *i != nullptr; i++)
		{
			auto filePath = boost::filesystem::path(*i);
			bool process = false;

			if (extensions.empty() ||
				extensions.contains(".*") ||
				extensions.contains("*.*"))
			{
				process = true;
			}
			else if (filePath.has_extension())
			{
				if (extensions.empty() ||
					extensions.contains(".*") ||
					extensions.contains("*.*") ||
					extensions.contains(filePath.extension().generic_string()))
				{
					process = true;
				}
			}

			if (process)
			{
				FileProperties fileProps = {};
				std::shared_ptr<PHYSFS_Stat> stat = std::make_shared<PHYSFS_Stat>();
				if (!PHYSFS_stat(*i, stat.get()))
				{
					LOG(error) << "Failed to get file properties: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				}

				if (stat)
				{
					fileProps.fileSize = stat->filesize;
					fileProps.lastModified = stat->modtime;
					fileProps.lastAccessed = stat->accesstime;
					fileProps.creationTime = stat->createtime;
					fileProps.isDirectory = (stat->filetype == PHYSFS_FILETYPE_DIRECTORY);
					fileProps.isSymlink = (stat->filetype == PHYSFS_FILETYPE_SYMLINK);
					fileProps.readOnly = stat->readonly;
				}

				fileProps.searchPath = boost::filesystem::path(PHYSFS_getRealDir(*i)).lexically_normal().generic_path();

				fileList[filePath] = fileProps;
			}
		}
		PHYSFS_freeList(static_cast<void*>(fileListing));

		return fileList;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return {};
}

std::map<boost::filesystem::path, common::filesystem::FileProperties> common::filesystem::FileSystem::GetFileList(const boost::filesystem::path& path,
	const sz::string& extension)
{
	return GetFileList(path, { extension });
}

std::map<boost::filesystem::path, common::filesystem::FileProperties> common::filesystem::FileSystem::GetFileList(const boost::filesystem::path& path)
{
	return GetFileList(path, std::set<sz::string>());
}

std::shared_ptr<common::filesystem::File> common::filesystem::FileSystem::OpenFile(const boost::filesystem::path& path, FileMode mode) const
{
	LOG_FUNC();

	try
	{
		if (path.empty())
		{
			LOG(error) << "File path is empty.";
			return nullptr;
		}

		if (!m_initialized)
		{
			LOG(error) << "File System is not initialized.";
			return nullptr;
		}

		auto selectedPath = path.lexically_normal().generic_path();
		if (m_fileLinks.contains(selectedPath))
		{
			selectedPath = m_fileLinks.at(selectedPath);  // Redirect
		}

		auto cFilePath = selectedPath.lexically_normal().generic_string().c_str();
		auto file = std::make_shared<File>();
		file->m_filePath = selectedPath.lexically_normal().generic_path();
		file->m_mode = mode;

		switch (mode)
		{
		case FileMode::FS_WRITE:
			file->m_handle = std::make_unique<PHYSFS_File>(PHYSFS_openWrite(cFilePath));
			break;
		case FileMode::FS_APPEND:
			file->m_handle = std::make_unique<PHYSFS_File>(PHYSFS_openAppend(cFilePath));
			break;
		default:
		case FileMode::FS_READ:
			// Check if file exists before opening in read mode
			if (!PHYSFS_exists(path.generic_string().c_str()))
			{
				LOG(error) << "File does not exist: " << path.generic_string();
				return nullptr;
			}

			file->m_handle = std::make_unique<PHYSFS_File>(PHYSFS_openRead(cFilePath));
			break;
		}

		if (file->m_handle == nullptr)
		{
			LOG(error) << "Failed to open file: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return nullptr;
		}

		LOG(info) << "Opened file '" << file->GetFileName() << "' in mode '" << magic_enum::enum_name(file->m_mode) << "'.";

		if (cvar_fsBufferSizeBytes && (cvar_fsBufferSizeBytes->UInt64() > 0))
		{
			size_t bufferBytes = static_cast<size_t>(cvar_fsBufferSizeBytes->UInt64());

			if (!PHYSFS_setBuffer(file->m_handle.get(), bufferBytes))
			{
				LOG(warning) << "Failed to set buffer of '" << bufferBytes << "': " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			}
		}

		if (file->m_mode == FileMode::FS_READ)
		{
			file->DetectFileEncoding();
		}
		else
		{
			file->m_encoding = Encoding::UTF8;
		}

		file->UpdateProperties();

		return file;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return nullptr;
}

std::shared_ptr<common::filesystem::File> common::filesystem::FileSystem::OpenFile(const sz::string& path, FileMode mode) const
{
	return OpenFile(boost::filesystem::path(path.c_str()), mode);
}

std::shared_ptr<common::filesystem::File> common::filesystem::FileSystem::OpenFile(const char* path, FileMode mode) const
{
	return OpenFile(boost::filesystem::path(path), mode);
}

void common::filesystem::FileSystem::CloseFile(std::shared_ptr<File>& file)
{
	LOG_FUNC();

	try
	{
		if (file == nullptr)
		{
			LOG(error) << "File is null.";
			return;
		}

		if ((file->m_mode == FileMode::FS_WRITE) || (file->m_mode == FileMode::FS_APPEND))
		{
			file->Flush();
		}

		if (file->m_handle != nullptr)
		{
			if (!PHYSFS_close(file->m_handle.get()))
			{
				LOG(error) << "Failed to close file: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			}
			else
			{
				LOG(info) << "Closed file: " << file->GetFileName();
			}

			file->m_handle = nullptr;
		}

		file->m_filePath.clear();

		file->m_mode = FileMode::FS_READ;

		file.reset();
		file = nullptr;
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}


