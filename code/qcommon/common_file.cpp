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

void common::filesystem::File::DetectFileEncoding()
{
	LOG_FUNC();

	m_encoding = UNKNOWN;

	try
	{
		if (m_mode != FileMode::FS_READ)
		{
			LOG(error) << "File is not open for reading.";
			return;
		}

		if (!PHYSFS_seek(m_handle.get(), 0))
		{
			LOG(error) << "Failed to seek to beginning of file: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return;
		}

		std::vector<byte> bom(4);
		if (PHYSFS_readBytes(m_handle.get(), reinterpret_cast<void*>(bom.data()), bom.size()) <= 0)
		{
			LOG(error) << "Failed to read BOM from file: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			return;
		}

		m_encoding = static_cast<Encoding>(simdutf::autodetect_encoding(bom));
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

void common::filesystem::File::UpdateProperties()
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return;
		}

		auto cFilePath = m_filePath.string().c_str();

		std::shared_ptr<PHYSFS_Stat> stat = std::make_shared<PHYSFS_Stat>();
		if (!PHYSFS_stat(cFilePath, stat.get()))
		{
			LOG(error) << "Failed to get file properties: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		}

		if (stat)
		{
			m_properties.fileSize = stat->filesize;
			m_properties.lastModified = stat->modtime;
			m_properties.lastAccessed = stat->accesstime;
			m_properties.creationTime = stat->createtime;
			m_properties.isDirectory = (stat->filetype == PHYSFS_FILETYPE_DIRECTORY);
			m_properties.isSymlink = (stat->filetype == PHYSFS_FILETYPE_SYMLINK);
			m_properties.readOnly = stat->readonly;
		}

		m_properties.searchPath = boost::filesystem::path(PHYSFS_getRealDir(cFilePath)).lexically_normal().generic_path();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

size_t common::filesystem::File::Read(void* buffer, const size_t bufferSize) const
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return 0;
		}

		if (!buffer)
		{
			LOG(warning) << "Buffer is null.";
			return 0;
		}

		if (bufferSize == 0)
		{
			LOG(warning) << "Buffer size is 0.";
			return 0;
		}

		if (m_handle)
		{
			if (m_mode == FileMode::FS_READ)
			{
				auto bytesRead = PHYSFS_readBytes(m_handle.get(), buffer, bufferSize);

				if (bytesRead <= 0)
				{
					LOG(error) << "Failed to read '" << bufferSize << "' bytes: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
					return 0;
				}
				else if (bytesRead < bufferSize)
				{
					if (!PHYSFS_eof(m_handle.get()))
					{
						LOG(warning) << "Only read '" << bytesRead << "' / '" << bufferSize << "' bytes: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
					}
				}

				return static_cast<size_t>(bytesRead);
			}
			else
			{
				LOG(error) << "File is not open for reading.";
				return 0;
			}
		}
		else
		{
			LOG(error) << "File handle is null.";
			return 0;
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

size_t common::filesystem::File::Write(const void* buffer, const size_t bufferSize) const
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return 0;
		}

		if (!buffer)
		{
			LOG(warning) << "Buffer is null.";
			return 0;
		}

		if (bufferSize == 0)
		{
			LOG(warning) << "Buffer size is 0.";
			return 0;
		}

		if (bufferSize > INT64_MAX) // Signed int64 max ***on purpose***
		{
			LOG(warning) << "Buffer size is too large.";
			return 0;
		}

		if (m_handle)
		{
			if ((m_mode == FileMode::FS_WRITE) || (m_mode == FileMode::FS_APPEND))
			{
				auto bytesWritten = PHYSFS_writeBytes(m_handle.get(), buffer, bufferSize);

				if (bytesWritten <= 0)
				{
					LOG(error) << "Failed to write '" << bufferSize << "' bytes: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
					return 0;
				}
				else if (bytesWritten < bufferSize)
				{
					LOG(warning) << "Only wrote '" << bytesWritten << "' / '" << bufferSize << "' bytes!";
				}

				return static_cast<size_t>(bytesWritten);
			}
			else
			{
				LOG(error) << "File is not open for writing.";
				return 0;
			}
		}
		else
		{
			LOG(error) << "File handle is null.";
			return 0;
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

size_t common::filesystem::File::Length()
{
	// Filesize might update for files being modified

	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return 0;
		}

		if (m_handle)
		{
			auto length = PHYSFS_fileLength(m_handle.get());
			if (length < 0)
			{
				LOG(error) << "Failed to get file length: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				return 0;
			}

			m_properties.fileSize = static_cast<size_t>(length);

			return m_properties.fileSize;
		}
		else
		{
			LOG(error) << "File handle is null.";
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

time_t common::filesystem::File::Created() const
{
	// This shouldn't ever normally change, aside from some deliberate file properties hackery
	return m_properties.creationTime;
}

time_t common::filesystem::File::Modified()
{
	// DTG might update for files being modified

	LOG_FUNC();

	try
	{
		UpdateProperties();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return m_properties.lastModified;
}

time_t common::filesystem::File::Accessed()
{
	// DTG might update

	LOG_FUNC();

	try
	{
		UpdateProperties();
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return m_properties.lastAccessed;
}

void common::filesystem::File::UpdateChecksums(int checksumFeed)
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return;
		}

		if (m_handle)
		{
			size_t file_length = Length();

			if (file_length == 0)
			{
				LOG(error) << "File length is zero, cannot compute checksum.";
				return;
			}

			byte* buffer = static_cast<byte*>(Z_Malloc(file_length));

			if (!buffer)
			{
				LOG(error) << "Failed to allocate memory buffer.";
				return;
			}

			if (buffer)
			{
				if ((m_filePath.extension() == ".pk3") ||
					(m_filePath.extension() == ".pk4") ||
					(m_filePath.extension() == ".pkx") ||
					(m_filePath.extension() == ".pkz") ||
					(m_filePath.extension() == ".zip"))
				{
					// Read the whole file
					size_t bytes_read = 0;
					size_t bytes_remaining = file_length;
					unsigned short tries = 0;

					while (bytes_remaining > 0)
					{
						bytes_read = Read(buffer, bytes_remaining);

						if (bytes_read == 0)
						{
							// we might have been trying to read from a CD
							if (!tries)
							{
								tries = 1;
							}
							else
							{
								return; // Com_Error(ERR_FATAL, "FS_Read: 0 bytes read");
							}
						}

						bytes_remaining -= bytes_read;
						buffer += bytes_read;
					}

					void* zip_handle = nullptr;
					size_t num_entries = 0;
					void* zip_reader = mz_zip_reader_create();

					if (!zip_reader)
					{
						LOG(error) << "Failed to create zip reader.";

						Z_Free(buffer);

						return;
					}

					if (mz_zip_reader_open_buffer(zip_handle, buffer, msl::utilities::SafeInt<int32_t>(file_length), 0) != MZ_OK)
					{
						LOG(error) << "Failed to open zip file from memory buffer.";
						
						mz_zip_reader_delete(&zip_reader);
						Z_Free(buffer);
						zip_handle = nullptr;

						return;
					}

					if (mz_zip_get_number_entry(zip_handle, &num_entries) != MZ_OK)
					{
						LOG(error) << "Failed to get number of entries in zip file.";

						mz_zip_reader_delete(&zip_reader);
						Z_Free(buffer);
						zip_handle = nullptr;

						return;
					}

					int* file_checksums = nullptr;
					size_t num_file_checksums = 0;

					file_checksums = static_cast<int*>(Z_Malloc((num_entries + 1) * sizeof(int)));
					file_checksums[num_file_checksums++] = LittleLong(checksumFeed);

					if (mz_zip_reader_goto_first_entry(zip_reader) != MZ_OK) 
					{
						LOG(error) << "Zip file '" << GetFilePath() << "' is empty!";

						mz_zip_reader_delete(&zip_reader);
						Z_Free(file_checksums);
						Z_Free(buffer);
						zip_handle = nullptr;

						return;
					}

					// Iterate through all entries
					do {
						// Get current entry information
						mz_zip_file* file_info = nullptr;

						if (mz_zip_reader_entry_get_info(zip_reader, &file_info) != MZ_OK)
						{
							LOG(error) << "Error getting entry info";
							break;
						}

						if (file_info->uncompressed_size > 0)
						{
							file_checksums[num_file_checksums++] = msl::utilities::SafeInt<int>(LittleULong(file_info->crc));
						}

					} while (mz_zip_reader_goto_next_entry(zip_reader) == MZ_OK);

					// Close and delete the zip reader
					mz_zip_reader_close(zip_reader);
					mz_zip_reader_delete(&zip_reader);
					zip_handle = nullptr;

					// Calculate checksums
					m_checksum = Com_BlockChecksum(&file_checksums[1], msl::utilities::SafeInt<int>(sizeof(*file_checksums) * (num_file_checksums - 1)));
					m_pureChecksum = Com_BlockChecksum(file_checksums, msl::utilities::SafeInt<int>(sizeof(*file_checksums) * num_file_checksums));
					m_checksum = LittleLong(m_checksum);
					m_pureChecksum = LittleLong(m_pureChecksum);

					Z_Free(file_checksums);
				}
				else if (m_filePath.extension() == ".pak")
				{
					dpakheader_t	header;
					dpakfile_t		info[MAX_FILES_IN_PAK];

					Seek(0, FS_SEEK_BEG);

					size_t header_bytes_read = Read(&header, sizeof(header));

					if (header_bytes_read != sizeof(header))
					{
						LOG(error) << "Failed to read pak header: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

						Z_Free(buffer);

						return;
					}

					if (LittleLong(header.ident) != IDPAKHEADER)
					{
						LOG(error) << GetFileName() << " is not a pak file.";

						Z_Free(buffer);

						return;
					}
					
					header.dirofs = LittleLong(header.dirofs);
					header.dirlen = LittleLong(header.dirlen);

					Seek(header.dirofs, FS_SEEK_SET);

					size_t info_bytes_read = Read(info, sizeof(info));

					if (info_bytes_read != header.dirlen)
					{
						LOG(error) << "Failed to read pak file info: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

						Z_Free(buffer);

						return;
					}

					m_checksum = Com_BlockChecksum(info, header.dirlen);
					m_checksum = LittleLong(m_checksum);
					m_pureChecksum = m_checksum;
				}
				else
				{
					// Checksum the whole file
					size_t bytes_read = 0;
					size_t bytes_remaining = file_length;
					unsigned short tries = 0;

					while (bytes_remaining > 0)
					{
						bytes_read = Read(buffer, bytes_remaining);

						if (bytes_read == 0)
						{
							// we might have been trying to read from a CD
							if (!tries)
							{
								tries = 1;
							}
							else
							{
								return; // Com_Error(ERR_FATAL, "FS_Read: 0 bytes read");
							}
						}

						bytes_remaining -= bytes_read;
						buffer += bytes_read;
					}

					m_checksum = Com_BlockChecksum(buffer, msl::utilities::SafeInt<int>(file_length));
					m_checksum = LittleLong(m_checksum);
					m_pureChecksum = m_checksum;
				}

				Z_Free(buffer);

				return;
			}
		}
		else
		{
			LOG(error) << "File handle is null.";
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return;
}

unsigned int common::filesystem::File::Checksum() const
{
	return m_checksum;
}

unsigned int common::filesystem::File::PureChecksum() const
{
	return m_pureChecksum;
}

boost::filesystem::path common::filesystem::File::GetParentSearchPath()
{
	return m_properties.searchPath;
}

void common::filesystem::File::Seek(int64_t offset, SeekOrigin origin) const
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return;
		}

		if (m_handle)
		{
			size_t position = 0;
			if (((origin == FS_SEEK_BEG) || (origin == FS_SEEK_SET)))
			{
				// Bound
				offset = std::max<int64_t>(offset, 0);
				position = std::min<size_t>(offset, m_properties.fileSize);
			}
			else if (origin == FS_SEEK_CUR)
			{
				size_t currentPosition = Tell();

				// Bound
				position = std::max<size_t>(currentPosition + offset, 0);
				position = std::min<size_t>(position, m_properties.fileSize);
			}
			else // FS_SEEK_END
			{
				position = std::max<size_t>(m_properties.fileSize - static_cast<size_t>(std::abs(offset)), 0);
				position = std::min<size_t>(position, m_properties.fileSize);
			}

			if (!PHYSFS_seek(m_handle.get(), position))
			{
				LOG(error) << "Failed to get seek to '" << offset << "': " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
			}
		}
		else
		{
			LOG(error) << "File handle is null.";
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

size_t common::filesystem::File::Tell() const
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
			return 0;
		}

		if (m_handle)
		{
			auto position = PHYSFS_tell(m_handle.get());
			if (position < 0)
			{
				LOG(error) << "Failed to get file position: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				return 0;
			}

			return static_cast<size_t>(position);
		}
		else
		{
			LOG(error) << "File handle is null.";
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

void common::filesystem::File::Flush() const
{
	LOG_FUNC();

	try
	{
		if (!PHYSFS_isInit())
		{
			LOG(error) << "PhysFS is not initialized.";
		}

		if (m_handle)
		{
			if ((m_mode == FileMode::FS_WRITE) || (m_mode == FileMode::FS_APPEND))
			{
				if (!PHYSFS_flush(m_handle.get()))
				{
					LOG(error) << "Failed to flush: " << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
				}
			}
			else
			{
				LOG(error) << "File is not open for writing.";
			}
		}
		else
		{
			LOG(error) << "File handle is null.";
		}
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}
}

#if defined(__int128)
inline size_t common::filesystem::File::ReadInt128(__int128& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadUnsignedInt128(unsigned __int128& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}
#endif

inline size_t common::filesystem::File::ReadInt64(int64_t& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadUnsignedInt64(uint64_t& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadInt(int& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadUnsignedInt(unsigned int& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadShort(short& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadUnsignedShort(unsigned short& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadChar(char& value)
{
	return Read(&value, sizeof(value));
}

inline size_t common::filesystem::File::ReadUnsignedChar(unsigned char& value)
{
	return Read(&value, sizeof(value));
}

inline size_t common::filesystem::File::ReadFloat(float& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadDouble(double& value)
{
	const size_t result = Read(&value, sizeof(value));
	boost::endian::little_to_native_inplace(value);
	return result;
}

inline size_t common::filesystem::File::ReadBool(bool& value)
{
	unsigned char c;
	const size_t result = ReadUnsignedChar(c);
	value = c ? true : false;
	return result;
}

inline size_t common::filesystem::File::ReadString(std::string& string)
{
	LOG_FUNC();

	string.resize(256);

	if ((m_encoding == UNKNOWN) || (m_encoding == UTF8))
	{
		char c = {};
		while (true) 
		{
			ReadChar(c);

			if (c == EOF || c == '\0')
			{
				break;
			}

			string.push_back(c);
		}
	}
	else if ((m_encoding == UTF16_BE) || (m_encoding == UTF16_LE))
	{
		uint16_t wc = 0;

		std::vector<char16_t> buffer(256);

		while (true)
		{
			ReadUnsignedShort(wc);

			if (wc == WEOF || wc == 0)
			{
				break;
			}

			buffer.push_back(wc);
		}

		if (m_encoding == UTF16_BE)
		{
			const size_t expected_utf8_words = simdutf::utf8_length_from_utf16be(buffer.data(), buffer.size());

			const size_t utf16_be_words = simdutf::count_utf16be(buffer.data(), buffer.size());

			string.resize(expected_utf8_words + 1);

			const size_t utf8_words = simdutf::convert_utf16be_to_utf8(buffer.data(), utf16_be_words, string.data());
		}
		else // UTF16_LE
		{
			const size_t expectedUTF8words = simdutf::utf8_length_from_utf16le(buffer.data(), buffer.size());

			const size_t utf16_le_words = simdutf::count_utf16le(buffer.data(), buffer.size());

			string.resize(expectedUTF8words + 1);

			const size_t utf8_words = simdutf::convert_utf16le_to_utf8(buffer.data(), utf16_le_words, string.data());
		}
	}
	else if ((m_encoding == UTF32_BE) || (m_encoding == UTF32_LE))
	{
		uint32_t wc = 0;

		std::vector<char32_t> buffer(256);

		while (true) 
		{
			if (m_encoding == UTF32_BE)
			{
				const size_t result = Read(&wc, sizeof(wc));
				boost::endian::big_to_native_inplace(wc);
			}
			else // UTF32_LE
			{
				ReadUnsignedInt(wc);
			}

			if (wc == 0)
			{
				break;
			}

			buffer.push_back(wc);
		}

		const size_t expected_utf8_words = simdutf::utf8_length_from_utf32(buffer.data(), buffer.size());

		const size_t utf32_words = buffer.size() / sizeof(char32_t);

		string.resize(expected_utf8_words + 1);

		const size_t utf8_words = simdutf::convert_utf32_to_utf8(buffer.data(), utf32_words, string.data());
	}
	else
	{
		LOG(error) << "Unknown encoding.";
	}

	string.shrink_to_fit();
	return string.size();
}

inline size_t common::filesystem::File::ReadVec2(vec2_t& vec)
{
	const size_t result = Read(&vec, sizeof(vec));
	switch_endianness_bytes_little_to_native(&vec, sizeof(vec), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadVec3(vec3_t& vec)
{
	const size_t result = Read(&vec, sizeof(vec));
	switch_endianness_bytes_little_to_native(&vec, sizeof(vec), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadVec4(vec4_t& vec)
{
	const size_t result = Read(&vec, sizeof(vec));
	switch_endianness_bytes_little_to_native(&vec, sizeof(vec), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadVec5(vec5_t& vec)
{
	const size_t result = Read(&vec, sizeof(vec));
	switch_endianness_bytes_little_to_native(&vec, sizeof(vec), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadVec6(vec6_t& vec)
{
	const size_t result = Read(&vec, sizeof(vec));
	switch_endianness_bytes_little_to_native(&vec, sizeof(vec), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadMat2(mat2_t& mat)
{
	const size_t result = Read(&mat, sizeof(mat));
	switch_endianness_bytes_little_to_native(&mat, sizeof(mat), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadMat3(mat3_t& mat)
{
	const size_t result = Read(&mat, sizeof(mat));
	switch_endianness_bytes_little_to_native(&mat, sizeof(mat), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadMat4(mat4_t& mat)
{
	const size_t result = Read(&mat, sizeof(mat));
	switch_endianness_bytes_little_to_native(&mat, sizeof(mat), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadMat5(mat5_t& mat)
{
	const size_t result = Read(&mat, sizeof(mat));
	switch_endianness_bytes_little_to_native(&mat, sizeof(mat), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::ReadMat6(mat6_t& mat)
{
	const size_t result = Read(&mat, sizeof(mat));
	switch_endianness_bytes_little_to_native(&mat, sizeof(mat), sizeof(float));
	return result;
}

inline size_t common::filesystem::File::WriteInt64(const int64_t value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteUnsignedInt64(const uint64_t value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteInt(const int value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteUnsignedInt(const unsigned int value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteShort(const short value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteUnsignedShort(const unsigned short value)
{
	const auto littleValue = boost::endian::native_to_little(value);
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteChar(const char value)
{
	const size_t result = Write(&value, sizeof(value));
	return result;
}

inline size_t common::filesystem::File::WriteUnsignedChar(const unsigned char value)
{
	const size_t result = Write(&value, sizeof(value));
	return result;
}

inline size_t common::filesystem::File::WriteFloat(const float value)
{
	//const auto littleValue = boost::endian::native_to_little(value);
	//const size_t result = Write(&littleValue, sizeof(littleValue));
	const size_t result = Write(&value, sizeof(value));
	return result;
}

inline size_t common::filesystem::File::WriteDouble(const double value)
{
	//const auto littleValue = boost::endian::native_to_little(value);
	//const size_t result = Write(&littleValue, sizeof(littleValue));
	const size_t result = Write(&value, sizeof(value));
	return result;
}

inline size_t common::filesystem::File::WriteBool(const bool value)
{
	const unsigned char c = value ? 1 : 0;
	const size_t result = WriteUnsignedChar(c);
	return result;
}

inline size_t common::filesystem::File::WriteString(const char* string)
{
	if (string)
	{
		return WriteString(std::string(string));
	}

	return 0;
}

inline size_t common::filesystem::File::WriteString(const wchar_t* string)
{
	return WriteString(ToWideString(string));
}

inline size_t common::filesystem::File::WriteString(const std::wstring& string)
{
	LOG_FUNC();

	if ((m_encoding == UNKNOWN) || (m_encoding == UTF8))
	{
		std::string str = ToString(string);

		return WriteString(str);
	}
	else if ((m_encoding == UTF16_BE) || (m_encoding == UTF16_LE))
	{
		std::vector<char16_t> buffer(string.size() + 1);
		if (m_encoding == UTF16_BE)
		{
			if (boost::endian::order::native == boost::endian::order::big)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16BE
					return Write(string.c_str(), string.size() * sizeof(char16_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32BE
					const size_t utf16beWords = simdutf::convert_utf32_to_utf16be(reinterpret_cast<const char32_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf16beWords * sizeof(char16_t));
				}
			}
			else // (boost::endian::order::native == boost::endian::order::little)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16LE, need UTF16BE
					simdutf::change_endianness_utf16(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), string.size() * sizeof(char16_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32LE, need UTF16BE
					const size_t utf16beWords = simdutf::convert_utf32_to_utf16be(reinterpret_cast<const char32_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf16beWords * sizeof(char16_t));
				}
			}
		}
		else // UTF16_LE
		{
			if (boost::endian::order::native == boost::endian::order::big)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16BE, need UTF16LE
					simdutf::change_endianness_utf16(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), string.size() * sizeof(char16_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32BE, need UTF16LE
					const size_t utf16leWords = simdutf::convert_utf32_to_utf16le(reinterpret_cast<const char32_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf16leWords * sizeof(char16_t));
				}
			}
			else // (boost::endian::order::native == boost::endian::order::little)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16LE
					return Write(string.data(), string.size() * sizeof(char16_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32LE, need UTF16LE
					const size_t utf16leWords = simdutf::convert_utf32_to_utf16le(reinterpret_cast<const char32_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf16leWords * sizeof(char16_t));
				}
			}
		}
	}
	else if ((m_encoding == UTF32_BE) || (m_encoding == UTF32_LE))
	{
		std::vector<char32_t> buffer(string.size() + 1);

		if (m_encoding == UTF32_BE)
		{
			if (boost::endian::order::native == boost::endian::order::big)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16BE, need UTF32BE
					const size_t utf32beWords = simdutf::convert_utf16be_to_utf32(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf32beWords * sizeof(char32_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32BE
					return Write(string.data(), string.size() * sizeof(char32_t));
				}
			}
			else // (boost::endian::order::native == boost::endian::order::little)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16LE, need UTF32BE
					const size_t utf32beWords = simdutf::convert_utf16le_to_utf32(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());

					for (auto& c : buffer)
					{
						boost::endian::native_to_big_inplace(c);
					}

					return Write(buffer.data(), utf32beWords * sizeof(char32_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32LE, need UTF32BE
					memcpy(buffer.data(), string.c_str(), string.size() * sizeof(char32_t));

					for (auto& c : buffer)
					{
						boost::endian::native_to_big_inplace(c);
					}

					return Write(buffer.data(), string.size() * sizeof(char32_t));
				}
			}
		}
		else // UTF32_LE
		{
			if (boost::endian::order::native == boost::endian::order::big)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16BE, need UTF32LE
					const size_t utf32leWords = simdutf::convert_utf16be_to_utf32(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());

					for (auto& c : buffer)
					{
						boost::endian::native_to_little_inplace(c);
					}

					return Write(buffer.data(), utf32leWords * sizeof(char32_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32BE, need UTF32LE
					memcpy(buffer.data(), string.c_str(), string.size() * sizeof(char32_t));

					for (auto& c : buffer)
					{
						boost::endian::native_to_little_inplace(c);
					}

					return Write(buffer.data(), string.size() * sizeof(char32_t));
				}
			}
			else // (boost::endian::order::native == boost::endian::order::little)
			{
				if (sizeof(wchar_t) == sizeof(char16_t))
				{
					// String is UTF16LE, need UTF32LE
					const size_t utf32leWords = simdutf::convert_utf16le_to_utf32(reinterpret_cast<const char16_t*>(string.c_str()), string.size(), buffer.data());
					return Write(buffer.data(), utf32leWords * sizeof(char32_t));
				}
				else if (sizeof(wchar_t) == sizeof(char32_t))
				{
					// String is UTF32LE
					return Write(string.c_str(), string.size() * sizeof(char32_t));
				}
			}
		}
	}
	else
	{
		LOG(error) << "Unknown encoding.";
	}
	return 0;
}

inline size_t common::filesystem::File::WriteString(const sz::string& string)
{
	return WriteString(std::string(string));
}

inline size_t common::filesystem::File::WriteString(const std::string& string)
{
	LOG_FUNC();

	if ((m_encoding == UNKNOWN) || (m_encoding == UTF8))
	{
		const size_t result = Write(string.c_str(), string.size());
		return result;
	}
	else if ((m_encoding == UTF16_BE) || (m_encoding == UTF16_LE))
	{
		std::vector<char16_t> buffer(string.size() + 1);
		if (m_encoding == UTF16_BE)
		{
			const size_t utf16beWords = simdutf::convert_utf8_to_utf16be(string.c_str(), string.size(), buffer.data());
			return Write(buffer.data(), utf16beWords * sizeof(char16_t));
		}
		else // UTF16_LE
		{
			const size_t utf16leWords = simdutf::convert_utf8_to_utf16le(string.c_str(), string.size(), buffer.data());
			return Write(buffer.data(), utf16leWords * sizeof(char16_t));
		}
	}
	else if ((m_encoding == UTF32_BE) || (m_encoding == UTF32_LE))
	{
		std::vector<char32_t> buffer(string.size() + 1);

		if (m_encoding == UTF32_BE)
		{
			const size_t utf32beWords = simdutf::convert_utf8_to_utf32(string.c_str(), string.size(), buffer.data());

			for (auto& c : buffer)
			{
				boost::endian::native_to_big_inplace(c);
			}

			return Write(buffer.data(), utf32beWords * sizeof(uint32_t));
		}
		else // UTF32_LE
		{
			const size_t utf32leWords = simdutf::convert_utf8_to_utf32(string.c_str(), string.size(), buffer.data());

			for (auto& c : buffer)
			{
				boost::endian::native_to_little_inplace(c);
			}

			return Write(buffer.data(), utf32leWords * sizeof(uint32_t));
		}
	}
	else
	{
		LOG(error) << "Unknown encoding.";
	}
	return 0;
}

inline size_t common::filesystem::File::WriteString(const sz::string_view& string)
{
	return WriteString(std::string(string));
}

inline size_t common::filesystem::File::WriteString(const std::string_view& string)
{
	return WriteString(std::string(string));
}

inline size_t common::filesystem::File::WriteVec2(const vec2_t& vec)
{
	vec2_t littleValue;
	memcpy(&littleValue, &vec, sizeof(vec));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteVec3(const vec3_t& vec)
{
	vec3_t littleValue;
	memcpy(&littleValue, &vec, sizeof(vec));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteVec4(const vec4_t& vec)
{
	vec4_t littleValue;
	memcpy(&littleValue, &vec, sizeof(vec));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteVec5(const vec5_t& vec)
{
	vec5_t littleValue;
	memcpy(&littleValue, &vec, sizeof(vec));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteVec6(const vec6_t& vec)
{
	vec6_t littleValue;
	memcpy(&littleValue, &vec, sizeof(vec));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteMat2(const mat2_t& mat)
{
	mat2_t littleValue;
	memcpy(&littleValue, &mat, sizeof(mat));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteMat3(const mat3_t& mat)
{
	mat3_t littleValue;
	memcpy(&littleValue, &mat, sizeof(mat));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteMat4(const mat4_t& mat)
{
	mat4_t littleValue;
	memcpy(&littleValue, &mat, sizeof(mat));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteMat5(const mat5_t& mat)
{
	mat5_t littleValue;
	memcpy(&littleValue, &mat, sizeof(mat));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}

inline size_t common::filesystem::File::WriteMat6(const mat6_t& mat)
{
	mat6_t littleValue;
	memcpy(&littleValue, &mat, sizeof(mat));
	switch_endianness_bytes_native_to_little(&littleValue, sizeof(littleValue), sizeof(float));
	const size_t result = Write(&littleValue, sizeof(littleValue));
	return result;
}