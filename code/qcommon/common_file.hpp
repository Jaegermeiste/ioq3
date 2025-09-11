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

#ifndef COMMON_FILE_HPP_
#define COMMON_FILE_HPP_
#pragma once

#include "common_filesystem.hpp"
#include "common_local.hpp"

namespace common
{
	const auto UTF8_LOCALE = boost::locale::generator().generate("en_US.UTF-8");
	const auto UTF16_LOCALE = boost::locale::generator().generate("en_US.UTF-16");
	const auto UTF32_LOCALE = boost::locale::generator().generate("en_US.UTF-32");

	enum Encoding
	{
		UNKNOWN = 0,
		UTF8 = 1,
		UTF16_LE = 2,
		UTF16_BE = 4,
		UTF32_LE = 8,
		UTF32_BE = 16,
	};

	namespace filesystem
	{
		enum SeekOrigin
		{
			FS_SEEK_BEG = 0,
			FS_SEEK_CUR = 1,
			FS_SEEK_END = 2,
			FS_SEEK_SET = 3
		};

		constexpr int IDPAKHEADER = (('K' << 24) + ('C' << 16) + ('A' << 8) + 'P');
		constexpr size_t MAX_FILES_IN_PAK = 4096;

		typedef struct dpakfile_s
		{
			char	name[56];
			int		filepos, filelen;
		} dpakfile_t;

		typedef struct dpakheader_s
		{
			int		ident;		// == IDPAKHEADER
			int		dirofs;
			int		dirlen;
		} dpakheader_t;

		class File
		{
			friend class FileSystem;

			boost::filesystem::path m_filePath;

			std::unique_ptr<PHYSFS_File> m_handle = nullptr;

			FileMode m_mode;

			FileProperties m_properties = {};

			Encoding m_encoding = Encoding::UNKNOWN;

			unsigned int m_checksum = 0;
			unsigned int m_pureChecksum = 0;

			void DetectFileEncoding();
			void UpdateProperties();
			void UpdateChecksums(int checksumFeed);

		public:
			File();

			[[nodiscard]] auto GetFilePath() const { return m_filePath; }

			[[nodiscard]] auto GetFileName() const
			{
				return m_filePath.filename();
			}
			[[nodiscard]] auto GetFileNameWithoutExtension() const
			{
				return m_filePath.stem();
			}
			[[nodiscard]] auto GetFileExtension() const
			{
				return m_filePath.extension();
			}

			size_t Read(void* buffer, const size_t bufferSize) const;
			size_t Write(const void* buffer, const size_t bufferSize) const;

			size_t Length();

			time_t Created() const;
			time_t Modified();
			time_t Accessed();

			unsigned int Checksum() const;

			unsigned int PureChecksum() const;

			boost::filesystem::path GetParentSearchPath();

			void   Seek(int64_t offset, SeekOrigin origin) const;
			size_t Tell() const;

			void   Flush() const;

			template<typename T, typename... Args>
			void Printf(T formatString, Args&&... args)
			{
				LOG_FUNC();

				try
				{
					std::string outStr;

					if (sizeof...(args) == 0)
					{
						outStr = ToString(formatString);
					}
					else
					{
						outStr = Format(formatString, std::forward<Args>(args)...);
					}

					// Add a newline character at the end
					outStr += "\n";

					WriteString(outStr);
				}
				catch (const std::runtime_error& e) {
					LOG(error) << "Runtime Error: " << e.what();
				}
				catch (const std::exception& e) {
					LOG(error) << "General Exception: " << e.what();
				}
			}

			// Endian portable alternatives to Read(...)
#if defined(__int128)
			virtual size_t				ReadInt128(__int128& value);
			virtual size_t				ReadUnsignedInt128(unsigned __int128& value);
#endif
			virtual size_t				ReadInt64(int64_t& value);
			virtual size_t				ReadUnsignedInt64(uint64_t& value);
			virtual size_t				ReadInt(int& value);
			virtual size_t				ReadUnsignedInt(unsigned int& value);
			virtual size_t				ReadShort(short& value);
			virtual size_t				ReadUnsignedShort(unsigned short& value);
			virtual size_t				ReadChar(char& value);
			virtual size_t				ReadUnsignedChar(unsigned char& value);
			virtual size_t				ReadFloat(float& value);
			virtual size_t				ReadDouble(double& value);
			virtual size_t				ReadBool(bool& value);
			virtual size_t				ReadString(std::string& string);
			virtual size_t				ReadVec2(vec2_t& vec);
			virtual size_t				ReadVec3(vec3_t& vec);
			virtual size_t				ReadVec4(vec4_t& vec);
			virtual size_t				ReadVec5(vec5_t& vec);
			virtual size_t				ReadVec6(vec6_t& vec);
			virtual size_t				ReadMat2(mat2_t& mat);
			virtual size_t				ReadMat3(mat3_t& mat);
			virtual size_t				ReadMat4(mat4_t& mat);
			virtual size_t				ReadMat5(mat5_t& mat);
			virtual size_t				ReadMat6(mat6_t& mat);

			// Endian portable alternatives to Write(...)
			virtual size_t				WriteInt64(const int64_t value);
			virtual size_t				WriteUnsignedInt64(const uint64_t value);
			virtual size_t				WriteInt(const int value);
			virtual size_t				WriteUnsignedInt(const unsigned int value);
			virtual size_t				WriteShort(const short value);
			virtual size_t				WriteUnsignedShort(unsigned short value);
			virtual size_t				WriteChar(const char value);
			virtual size_t				WriteUnsignedChar(const unsigned char value);
			virtual size_t				WriteFloat(const float value);
			virtual size_t				WriteDouble(const double value);
			virtual size_t				WriteBool(const bool value);
			virtual size_t				WriteString(const char* string);
			virtual size_t				WriteString(const wchar_t* string);
			virtual size_t				WriteString(const std::wstring& string);
			virtual size_t				WriteString(const sz::string& string);
			virtual size_t				WriteString(const std::string& string);
			virtual size_t				WriteString(const sz::string_view& string);
			virtual size_t				WriteString(const std::string_view& string);
			virtual size_t				WriteVec2(const vec2_t& vec);
			virtual size_t				WriteVec3(const vec3_t& vec);
			virtual size_t				WriteVec4(const vec4_t& vec);
			virtual size_t				WriteVec5(const vec5_t& vec);
			virtual size_t				WriteVec6(const vec6_t& vec);
			virtual size_t				WriteMat2(const mat2_t& mat);
			virtual size_t				WriteMat3(const mat3_t& mat);
			virtual size_t				WriteMat4(const mat4_t& mat);
			virtual size_t				WriteMat5(const mat5_t& mat);
			virtual size_t				WriteMat6(const mat6_t& mat);
		};

	}
}

#endif // !COMMON_FILE_HPP_