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

#ifndef COMMON_FILESYSTEM_HPP_
#define COMMON_FILESYSTEM_HPP_
#pragma once

#include "common_local.hpp"

namespace common
{
	namespace filesystem
	{
		class File;

		enum class FileMode : byte
		{
			FS_READ = 0,
			FS_WRITE = 1,
			FS_APPEND = 2
		};

		inline std::shared_ptr<CVar> cvar_fsBasedir;
		inline std::shared_ptr<CVar> cvar_fsSavePath;
		inline std::shared_ptr<CVar> cvar_fsCDDir;
		inline std::shared_ptr<CVar> cvar_fsEnableDiscs;
		inline std::shared_ptr<CVar> cvar_fsBufferSizeBytes;

		struct FileProperties
		{
			boost::filesystem::path searchPath;
			size_t fileSize = 0;
			time_t lastModified = 0;
			time_t lastAccessed = 0;
			time_t creationTime = 0;
			bool isDirectory = false;
			bool isSymlink = false;
			bool readOnly = false;
		};

		class FileSystem
		{
		private:
			bool m_initialized = false;

			boost::filesystem::path m_basePath = {};
			boost::filesystem::path m_savePath = {};

			std::unordered_map<boost::filesystem::path, boost::filesystem::path> m_fileLinks;

		public:
			bool						Initialize();
			void						Shutdown();

			[[nodiscard]] bool IsInitialized() const { return m_initialized; }

			[[nodiscard]] boost::filesystem::path GetBasePath() { return m_basePath; };
			[[nodiscard]] boost::filesystem::path GetSavePath() { return m_savePath; };

			static bool AddSearchPath(const boost::filesystem::path& path, const bool append = true);
			static bool AddSearchPath(const sz::string& path, const bool append = true);
			static bool AddSearchPath(const char* path, const bool append = true);

			bool AddDefaultSearchPaths() const;

			static bool RemoveSearchPath(const boost::filesystem::path& path);
			static bool RemoveSearchPath(const sz::string& path);
			static bool RemoveSearchPath(const char* path);

			void RemoveAllSearchPaths() const;

			[[nodiscard]] std::vector<boost::filesystem::path> GetSearchPaths() const;

			[[nodiscard]] bool MakeDirectory(const boost::filesystem::path& path) const;
			[[nodiscard]] bool MakeDirectory(const sz::string& filePath) const;
			[[nodiscard]] bool MakeDirectory(const char* filePath) const;

			[[nodiscard]] bool FileExists(const boost::filesystem::path& filePath) const;
			[[nodiscard]] bool FileExists(const sz::string& filePath) const;
			[[nodiscard]] bool FileExists(const char* filePath) const;

			bool MakeFileLink(const boost::filesystem::path& from, const boost::filesystem::path& to);
			bool MakeFileLink(const sz::string& from, const sz::string& to);
			bool MakeFileLink(const char* from, const char* to);
			bool RemoveFileLink(const boost::filesystem::path& from);
			std::unordered_map<boost::filesystem::path, boost::filesystem::path> GetFileLinks();

			[[nodiscard]] static std::map<boost::filesystem::path, FileProperties> GetFileList(const boost::filesystem::path& path, const std::set<sz::string>& extensions);
			[[nodiscard]] static std::map<boost::filesystem::path, FileProperties> GetFileList(const boost::filesystem::path& path, const sz::string& extension);
			[[nodiscard]] static std::map<boost::filesystem::path, FileProperties> GetFileList(const boost::filesystem::path& path);

			[[nodiscard]] std::shared_ptr<File> OpenFile(const boost::filesystem::path& path, FileMode mode = FileMode::FS_READ) const;
			[[nodiscard]] std::shared_ptr<File> OpenFile(const sz::string& path, FileMode mode = FileMode::FS_READ) const;
			[[nodiscard]] std::shared_ptr<File> OpenFile(const char* path, FileMode mode = FileMode::FS_READ) const;
			static void CloseFile(std::shared_ptr<File>& file);
		};
	}
}

#endif // !COMMON_FILESYSTEM_HPP_