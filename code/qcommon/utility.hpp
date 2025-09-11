/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
quake3 common
2025 Bleeding Eye Studios
*/

#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#pragma once

#include "common_local.hpp"

namespace utility
{
	extern boost::uuids::time_generator_v7 uuid7_gen;
};

/*namespace ashvardanian::stringzilla {

	//
 //  @brief 128‑entry table mapping each byte to its lowercase: ASCII 'A'–'Z' (0x41–0x5A) → 'a'–'z' (0x61–0x7A); all
 // other bytes unchanged.
 //
	inline carray<256> const& ascii_lowercase_all() noexcept {
		static ucarray<256> const all = {
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
			0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
			0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
			0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
			0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
			0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
			0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D,
			0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
			0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1,
			0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3,
			0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
			0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
			0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
			0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,
			0xFC, 0xFD, 0xFE, 0xFF };
		return static_cast<carray>(all);
	};

	//
	//  @brief 128‑entry table mapping each byte to its uppercase: ASCII 'a'–'z' (0x61–0x7A) → 'A'–'Z' (0x41–0x5A); all
	// other bytes unchanged.
	//
	inline carray<256> const& ascii_uppercase_all() noexcept {
		static ucarray<256> const all = {
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
			0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
			0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
			0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
			0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
			0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
			0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D,
			0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
			0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1,
			0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3,
			0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
			0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
			0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
			0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,
			0xFC, 0xFD, 0xFE, 0xFF };
		return static_cast<carray>(all);
	};

	static look_up_table ascii_lower_lut = look_up_table(ascii_lowercase_all());
	static look_up_table ascii_upper_lut = look_up_table(ascii_uppercase_all());

	// Return an ASCII lower case version of the input
	inline sz::string tolower(const sz::string_view& input) noexcept {
		sz::string output(input);
		return output.transform(ascii_lower_lut);
	}

	template <typename string>
	inline string tolower(const string& input) noexcept {
		string output(input);
		sz::basic_string_slice slice{ output.data(), output.size() };
		sz::transform(slice, ascii_lower_lut);
		return output;
	}

	// Return an ASCII lower case version of the input
	inline sz::string toupper(const sz::string_view& input) noexcept {
		sz::string output(input);
		return output.transform(ascii_upper_lut);
	}

	template <typename string>
	inline string toupper(const string& input) noexcept {
		string output(input);
		sz::basic_string_slice slice{ output.data(), output.size() };
		sz::transform(slice, ascii_upper_lut);
		return output;
	}
}*/

namespace boost {
	template<>
	inline bool lexical_cast<bool, std::string>(const std::string& arg) {
		std::istringstream ss(arg);
		bool b;
		ss >> std::ws >> std::boolalpha >> b;
		return b;
	}

	template<>
	inline std::string lexical_cast<std::string, bool>(const bool& b) {
		std::ostringstream ss;
		ss << std::boolalpha << b;
		return ss.str();
	}

	template<>
	inline bool lexical_cast<bool, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		bool b;
		ss >> std::ws >> std::boolalpha >> b;
		return b;
	}

	template<>
	inline sz::string lexical_cast<sz::string, bool>(const bool& b) {
		std::ostringstream ss;
		ss << std::boolalpha << b;
		return ss.str();
	}

	/*template <typename Target, typename Source>
	inline typename std::enable_if_t<(std::is_same_v<Source, sz::string> and
		(not std::is_same_v<Target, bool>) and
		(not std::is_same_v<Target, float>) and
		(not std::is_same_v<Target, double>)), Target> lexical_cast(const Source& arg)
	{
		Target result = Target();

		if (!boost::conversion::detail::try_lexical_convert(std::string(arg), result)) {
			boost::conversion::detail::throw_bad_cast<Source, Target>();
		}

		return result;
	}

	template <typename Target, typename Source>
	inline typename std::enable_if_t<(std::is_same_v<Target, sz::string> and
							  (not std::is_same_v<Source, bool>) and
							  (not std::is_same_v<Source, float>) and
							  (not std::is_same_v<Source, double>)), Target> lexical_cast(const Source& arg)

	{
		std::string result = std::string();

		if (!boost::conversion::detail::try_lexical_convert(arg, result)) {
			boost::conversion::detail::throw_bad_cast<Source, Target>();
		}

		return sz::string(result);
	}*/

	template<>
	inline int64_t lexical_cast<int64_t, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		int64_t value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline sz::string lexical_cast<sz::string, int64_t>(const int64_t& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline uint64_t lexical_cast<uint64_t, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		uint64_t value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline sz::string lexical_cast<sz::string, uint64_t>(const uint64_t& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline int lexical_cast<int, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		int64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<int>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, int>(const int& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline unsigned int lexical_cast<unsigned int, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		uint64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<unsigned int>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, unsigned int>(const unsigned int& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline short lexical_cast<short, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		int64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<short>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, short>(const short& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline unsigned short lexical_cast<unsigned short, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		uint64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<unsigned short>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, unsigned short>(const unsigned short& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline char lexical_cast<char, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		int64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<char>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, char>(const char& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	template<>
	inline unsigned char lexical_cast<unsigned char, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		uint64_t value;
		ss >> std::ws >> value;
		return msl::utilities::SafeInt<unsigned char>(value);
	}

	template<>
	inline sz::string lexical_cast<sz::string, unsigned char>(const unsigned char& arg) {
		std::ostringstream ss;
		ss << arg;
		return ss.str();
	}

	constexpr auto float_max_precision{ std::numeric_limits<float>::digits10 + 1 };

	template<>
	inline float lexical_cast<float, std::string>(const std::string& arg) {
		std::istringstream ss(arg);
		float value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline std::string lexical_cast<std::string, float>(const float& arg) {
		std::ostringstream ss;
		ss << std::setprecision(float_max_precision) << arg;
		return ss.str();
	}

	template<>
	inline float lexical_cast<float, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		float value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline sz::string lexical_cast<sz::string, float>(const float& arg) {
		std::ostringstream ss;
		ss << std::setprecision(float_max_precision) << arg;
		return ss.str();
	}

	constexpr auto double_max_precision{ std::numeric_limits<double>::digits10 + 1 };

	template<>
	inline double lexical_cast<double, std::string>(const std::string& arg) {
		std::istringstream ss(arg);
		double value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline std::string lexical_cast<std::string, double>(const double& arg) {
		std::ostringstream ss;
		ss << std::setprecision(double_max_precision) << arg;
		return ss.str();
	}

	template<>
	inline double lexical_cast<double, sz::string>(const sz::string& arg) {
		std::istringstream ss(arg);
		double value;
		ss >> std::ws >> value;
		return value;
	}

	template<>
	inline sz::string lexical_cast<sz::string, double>(const double& arg) {
		std::ostringstream ss;
		ss << std::setprecision(double_max_precision) << arg;
		return ss.str();
	}
}

template <typename T>
inline std::wstring ToWideString(const T& value)
{
	if (value == nullptr)
	{
		return {};
	}

	return std::to_wstring(value);
}

template <>
inline std::wstring ToWideString<bool>(const bool& value)
{
	return value ? L"True" : L"False";
}

template<>
inline std::wstring ToWideString<std::string>(const std::string& inStr)
{
	if (inStr.empty())
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<wchar_t>(inStr);
}

template<>
inline std::wstring ToWideString<const WCHAR>(const WCHAR& inWideChar)
{
	return std::wstring() + inWideChar;
}

template<std::size_t N>
inline std::wstring ToWideString(const WCHAR(&inWideStr)[N])
{
	return std::wstring(inWideStr);
}

template<>
inline std::wstring ToWideString<WCHAR*>(WCHAR* const& inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return std::wstring(inWideStr);
}

template<>
inline std::wstring ToWideString<const WCHAR*>(const WCHAR* const& inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return std::wstring(inWideStr);
}

template<>
inline std::wstring ToWideString<char>(const char& inStr)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(&inStr, &inStr);
}

template<std::size_t N>
inline std::wstring ToWideString(const char(&inStr)[N])
{
	return boost::locale::conv::utf_to_utf<wchar_t>(inStr);
}

template<>
inline std::wstring ToWideString<char*>(char* const& inStr)
{
	if (inStr == nullptr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<wchar_t>(inStr);
}

template<>
inline std::wstring ToWideString<const char*>(const char* const& inStr)
{
	if (inStr == nullptr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<wchar_t>(inStr);
}

template <>
inline std::wstring ToWideString<std::nullptr_t>(const std::nullptr_t&)
{
	return {};
}

inline const std::wstring& ToWideString(const std::wstring& inWideStr)
{
	return inWideStr;
}

template <typename T>
inline sz::string ToString(const T& value)
{
	/*if (value == nullptr)
	{
		return {};
	}*/

	return sz::string(boost::lexical_cast<std::string, T>(value));
}

template <>
inline sz::string ToString<bool>(const bool& value)
{
	return sz::string(boost::lexical_cast<std::string>(value));
}

template <>
inline sz::string ToString<float>(const float& value)
{
	return boost::lexical_cast<sz::string, float>(value);
}

template <>
inline sz::string ToString<double>(const double& value)
{
	return boost::lexical_cast<sz::string, double>(value);
}

template<>
inline sz::string ToString<std::string>(const std::string& inStr)
{
	if (inStr.empty())
	{
		return {};
	}

	return inStr;
}

template<std::size_t N>
inline sz::string ToString(const WCHAR(&inWideStr)[N])
{
	return std::wstring(inWideStr);
}

template<>
inline sz::string ToString<WCHAR*>(WCHAR* const& inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<char>(inWideStr);
}

template<>
inline sz::string ToString<const WCHAR*>(const WCHAR* const& inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<char>(inWideStr);
}

inline sz::string ToString(const std::wstring& inWideStr)
{
	if (inWideStr.empty())
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<char>(inWideStr);
}

inline sz::string ToString(const WCHAR* inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<char>(inWideStr);
}

inline sz::string ToString(const WCHAR& inWideStr)
{
	if (!inWideStr)
	{
		return {};
	}

	return boost::locale::conv::utf_to_utf<char>(&inWideStr);
}

template<>
inline sz::string ToString<char>(const char& inStr)
{
	return { &inStr };
}

template<std::size_t N>
inline sz::string ToString(const char(&inStr)[N])
{
	return sz::string(inStr);
}

template<>
inline sz::string ToString<char*>(char* const& inStr)
{
	if (inStr == nullptr)
	{
		return {};
	}

	return { inStr };
}

template<>
inline sz::string ToString<const char*>(const char* const& inStr)
{
	if (inStr == nullptr)
	{
		return {};
	}

	return { inStr };
}

template <>
inline sz::string ToString<std::nullptr_t>(const std::nullptr_t&)
{
	return {};
}

inline const sz::string& ToString(const std::string& inStr)
{
	return inStr;
}

inline const sz::string& ToString(const sz::string& inStr)
{
	return inStr;
}

// Wide
/*template <>
struct std::formatter<const char*, wchar_t> : std::formatter<const char*, char> {
	constexpr auto parse(std::wformat_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = L"";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::wstring wstr(value.begin(), value.end());

		std::wstring final_string = L"";

		if (!m_format.empty())
		{
			final_string = L"{:" + m_format + L"}";
		}
		else
		{
			final_string = L"{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_wformat_args(wstr));
	}

	mutable std::wstring m_format;
};

template <>
struct std::formatter<char*, wchar_t> : std::formatter<char*, char> {
	constexpr auto parse(std::wformat_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = L"";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::wstring wstr(value.begin(), value.end());

		std::wstring final_string;

		if (!m_format.empty())
		{
			final_string = L"{:" + m_format + L"}";
		}
		else
		{
			final_string = L"{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_wformat_args(wstr));
	}

	mutable std::wstring m_format;
};


template <>
struct std::formatter<std::string, wchar_t> : std::formatter<std::string, char> {
	constexpr auto parse(std::wformat_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = L"";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::wstring wstr(value.begin(), value.end());

		std::wstring final_string;

		if (!m_format.empty())
		{
			final_string = L"{:" + m_format + L"}";
		}
		else
		{
			final_string = L"{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_wformat_args(wstr));
	}

	mutable std::wstring m_format;
};

template <>
struct std::formatter<std::string_view, wchar_t> : std::formatter<std::string_view, char> {
	constexpr auto parse(std::wformat_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = L"";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::wstring wstr(value.begin(), value.end());

		std::wstring final_string;

		if (!m_format.empty())
		{
			final_string = L"{:" + m_format + L"}";
		}
		else
		{
			final_string = L"{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_wformat_args(wstr));
	}

	mutable std::wstring m_format;
};*/

// Narrow
template <>
struct std::formatter<const char*, wchar_t> : std::formatter<const char*, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<char*, wchar_t> : std::formatter<char*, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_wformat_args(str));
	}

	mutable std::string m_format;
};


template <>
struct std::formatter<std::string, wchar_t> : std::formatter<std::string, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<std::string_view, wchar_t> : std::formatter<std::string_view, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(std::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string_view, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string, const char> : std::formatter<sz::string, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string_view, const char> : std::formatter<sz::string_view, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string, wchar_t> : std::formatter<sz::string, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

template <>
struct std::formatter<sz::string_view, wchar_t> : std::formatter<sz::string_view, char> {
	constexpr auto parse(std::format_parse_context& ctx) const
	{
		auto it = ctx.begin();

		m_format = "";

		while (*it && *it != '}')
		{
			m_format += *it;
			++it;
		}
		return it;
	}

	template <typename FormatContext>
	auto format(sz::string_view value, FormatContext& ctx) const {
		std::string str(value.begin(), value.end());

		std::string final_string;

		if (!m_format.empty())
		{
			final_string = "{:" + m_format + "}";
		}
		else
		{
			final_string = "{}";
		}

		return std::vformat_to(ctx.out(), final_string, std::make_format_args(str));
	}

	mutable std::string m_format;
};

// Wrapper to handle strings properly
template <typename T, typename... Args>
sz::string Format(T fmt, Args&&... args) {
	return std::vformat(std::string(ToString(fmt)), std::make_format_args(args...));
}


inline static bool IsValidPointer(void* ptr)
{
	LOG_FUNC();

	if (ptr == nullptr)
	{
		return false;
	}

	try {
#ifdef _WIN32
		MEMORY_BASIC_INFORMATION mbi = {};
		if (VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		{
			return false; // Pointer is not valid
		}

		return mbi.State == MEM_COMMIT; // Ensures it's committed memory

#else
		long pageSize = sysconf(_SC_PAGESIZE);
		void* pageStart = (void*)((uintptr_t)ptr & ~(pageSize - 1));

		unsigned char status;
		if (mincore(pageStart, pageSize, &status) == 0)
			return true;
#endif
	}
	catch (const std::runtime_error& e) {
		LOG(error) << "Runtime Error: " << e.what();
	}
	catch (const std::exception& e) {
		LOG(error) << "General Exception: " << e.what();
	}

	return false;
}

inline static sz::string FormatBytes(size_t sizeBytes)
{
	static float gib = 1073741824.0f;
	static float mib = 1048576.0f;
	static float kib = 1024.0f;

	float fBytes = static_cast<float>(sizeBytes);

	if (fBytes > gib)
	{
		return Format("{:.3} GiB", fBytes / gib);
	}
	else if (fBytes > mib)
	{
		return Format("{:.3} MiB", fBytes / mib);
	}
	else if (fBytes > kib)
	{
		return Format("{:.3} KiB", fBytes / kib);
	}
	else
	{
		return Format("{} B", sizeBytes);
	}

	return "";
}

inline sz::string GetUUIDv7() { sz::string str = boost::uuids::to_string(utility::uuid7_gen()); return str; };

//Vector2 GetNormalizedDeviceCoordinates(int px, int py, int windowWidth, int windowHeight);

//Vector4 GetNormalizedDeviceRectangle(int px, int py, int pw, int ph, int windowWidth, int windowHeight);

template<boost::endian::order from, boost::endian::order to>
inline void switch_endianness_bytes(void* data, size_t byteCount, size_t stride)
{
	if (from == to)
	{
		return;                      // nothing to do
	}

	if (stride < 2)
	{
		return;                      // nothing to do
	}
	else if (stride > 16)
	{
		throw std::invalid_argument{ "stride must be less than or equal to 16" };
	}

	if (byteCount % stride != 0)
	{
		throw std::invalid_argument{ "byteCount must be multiple of stride" };
	}

	auto* p = static_cast<std::uint8_t*>(data);
	const size_t elems = byteCount / stride;

	for (size_t i = 0; i < elems; i++)
	{
		auto* start = p + i * stride;
		auto* finish = start + stride;
		std::reverse(start, finish);
	}
}

inline void switch_endianness_bytes_little_to_native(void* data, size_t byteCount, size_t stride)
{
	switch_endianness_bytes<boost::endian::order::little, boost::endian::order::native>(data, byteCount, stride);
}

inline void switch_endianness_bytes_native_to_little(void* data, size_t byteCount, size_t stride)
{
	switch_endianness_bytes<boost::endian::order::native, boost::endian::order::little>(data, byteCount, stride);
}

inline sz::string time_t_to_string(const time_t time)
{
	// Convert time_t to ptime
	auto pt = boost::posix_time::from_time_t(time);

	// Convert ptime to string
	return boost::posix_time::to_iso_extended_string(pt);
}

inline void SetFlag(unsigned int& flags, const unsigned int flag)
{
	flags |= flag;
}

inline void SetFlag(int& flags, const unsigned int flag)
{
	flags |= msl::utilities::SafeInt<int>(flag);
}

inline void ClearFlag(unsigned int& flags, const unsigned int flag)
{
	flags &= ~flag;
}

inline void ClearFlag(int& flags, const unsigned int flag)
{
	flags &= ~(msl::utilities::SafeInt<int>(flag));
}

inline bool IsFlagSet(const unsigned int& flags, const unsigned int flag)
{
	return (flags & flag) != 0;
}

inline bool IsFlagSet(const int& flags, const unsigned int flag)
{
	return (flags & msl::utilities::SafeInt<int>(flag)) != 0;
}

inline void SyncFlag(const unsigned int& sourceFlags, unsigned int& targetFlags, const unsigned int flag)
{
	if (IsFlagSet(targetFlags, flag) && !IsFlagSet(sourceFlags, flag))
	{
		// Target has, Source doesn't, so clear it on Target
		ClearFlag(targetFlags, flag);
	}
	else if (!IsFlagSet(targetFlags, flag) && IsFlagSet(sourceFlags, flag))
	{
		// Target doesn't, Source does, so set it on Target
		SetFlag(targetFlags, flag);
	}
	/*else
	{
		// Identical
	}*/
}

inline void SyncFlag(const unsigned int& sourceFlags, int& targetFlags, const unsigned int flag)
{
	if (IsFlagSet(targetFlags, flag) && !IsFlagSet(sourceFlags, flag))
	{
		// Target has, Source doesn't, so clear it on Target
		ClearFlag(targetFlags, flag);
	}
	else if (!IsFlagSet(targetFlags, flag) && IsFlagSet(sourceFlags, flag))
	{
		// Target doesn't, Source does, so set it on Target
		SetFlag(targetFlags, flag);
	}
	/*else
	{
		// Identical
	}*/
}

inline void SyncFlag(const int& sourceFlags, unsigned int& targetFlags, const unsigned int flag)
{
	if (IsFlagSet(targetFlags, flag) && !IsFlagSet(sourceFlags, flag))
	{
		// Target has, Source doesn't, so clear it on Target
		ClearFlag(targetFlags, flag);
	}
	else if (!IsFlagSet(targetFlags, flag) && IsFlagSet(sourceFlags, flag))
	{
		// Target doesn't, Source does, so set it on Target
		SetFlag(targetFlags, flag);
	}
	/*else
	{
		// Identical
	}*/
}

inline void SyncFlag(const int& sourceFlags, int& targetFlags, const unsigned int flag)
{
	if (IsFlagSet(targetFlags, flag) && !IsFlagSet(sourceFlags, flag))
	{
		// Target has, Source doesn't, so clear it on Target
		ClearFlag(targetFlags, flag);
	}
	else if (!IsFlagSet(targetFlags, flag) && IsFlagSet(sourceFlags, flag))
	{
		// Target doesn't, Source does, so set it on Target
		SetFlag(targetFlags, flag);
	}
	/*else
	{
		// Identical
	}*/
}

inline void RemoveSubstrings(sz::string& str, const std::set<sz::string>& substrings)
{
	for (const auto& sub : substrings)
	{
		if (sub.empty())
		{
			continue;                // skip empty patterns
		}

		std::size_t pos = 0;
		// Keep finding and erasing 'sub' until none remain
		while ((pos = str.find(sub, pos)) != sz::string::npos) {
			str.erase(pos, sub.length());
			// Note: do not advance pos, as removing may expose a new match at the same index
		}
	}
}

inline void RemoveSubstrings(sz::string& str, const std::set<const char*>& substrings)
{
	std::set<sz::string> new_substrings;

	for (const auto& sub : substrings)
	{
		new_substrings.insert(sz::string(sub));
	}

	RemoveSubstrings(str, new_substrings);
}

inline sz::string StripColors(const sz::string& input)
{
	static auto colors = { S_COLOR_DEFAULT, S_COLOR_RED, S_COLOR_GREEN, S_COLOR_YELLOW, S_COLOR_BLUE, S_COLOR_CYAN, S_COLOR_ORANGE, S_COLOR_WHITE, S_COLOR_GRAY, S_COLOR_BLACK };
	sz::string output = input;
	RemoveSubstrings(output, colors);

	return output;
}

/// Uppercase using a specific ICU Locale.
inline sz::string ToUpper(const sz::string& input, const icu::Locale& locale = icu::Locale::getUS())
{
	icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(input.c_str());
	unicode_str.toUpper(locale);
	sz::string output;
	unicode_str.toUTF8String(output);
	return output;
}

/// Uppercase using a locale name (e.g. "tr", "tr-TR", "de", "en_US").
inline sz::string ToUpper(const sz::string& input, const sz::string& localeName)
{
	return ToUpper(input, icu::Locale::createFromName(localeName.c_str()));
}

/// Lowercase using a specific ICU Locale.
inline sz::string ToLower(const sz::string& input, const icu::Locale& locale = icu::Locale::getUS())
{
	icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(input.c_str());
	unicode_str.toLower(locale);
	sz::string output;
	unicode_str.toUTF8String(output);
	return output;
}

/// Lowercase using a locale name.
inline sz::string ToLower(const sz::string& input, const sz::string& localeName)
{
	return ToLower(input, icu::Locale::createFromName(localeName.c_str()));
}

// Round up to next power of two (for v > 0)
static inline unsigned int NextPow2(unsigned int v)
{
	// if already a power of two, returns v
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return ++v;
}

template <class BaseType, size_t FracDigits>
class FixedPoint
{
private:
	const static BaseType m_factor = 1 << FracDigits;

	BaseType m_value;

public:
	explicit FixedPoint(const signed short ss)
	{
		*this = static_cast<double>(ss); // calls operator=
	}

	explicit FixedPoint(const unsigned short us)
	{
		*this = static_cast<double>(us); // calls operator=
	}

	explicit FixedPoint(const signed int si)
	{
		*this = static_cast<double>(si); // calls operator=
	}

	explicit FixedPoint(const unsigned int ui)
	{
		*this = static_cast<double>(ui); // calls operator=
	}

	explicit FixedPoint(const float f)
	{
		*this = static_cast<double>(f); // calls operator=
	}

	explicit FixedPoint(double d)
	{
		*this = d; // calls operator=
	}

	FixedPoint& operator=(double d)
	{
		m_value = static_cast<BaseType>(d * m_factor);
		return *this;
	}

	constexpr inline FixedPoint operator-() const noexcept
	{
		return FixedPoint(-m_value);
	}

	inline FixedPoint& operator+=(const FixedPoint& y) noexcept
	{
		m_value += y.m_value;
		return *this;
	}

	template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline FixedPoint& operator+=(T y) noexcept
	{
		m_value += y * m_factor;
		return *this;
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline FixedPoint& operator+=(T y) noexcept
	{
		this += FixedPoint(y);
		return *this;
	}

	inline FixedPoint& operator-=(const FixedPoint& y) noexcept
	{
		m_value -= y.m_value;
		return *this;
	}

	template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline FixedPoint& operator-=(T y) noexcept
	{
		m_value -= y * m_factor;
		return *this;
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline FixedPoint& operator-=(T y) noexcept
	{
		this -= FixedPoint(y);
		return *this;
	}

	// Equality operator
	template <typename T>
		requires(std::is_arithmetic_v<T>)
	constexpr auto operator==(T value) noexcept -> bool {
		return FixedPoint(value).m_value == this->m_value;
	}

	// Reverse equality operator
	/*template <typename T>
		requires(std::is_arithmetic_v<T>)
	constexpr auto operator==(T a, const FixedPoint& b) noexcept -> bool {
		return FixedPoint(a) == b;
	}*/

	// Conversion to a floating-point type
	template <typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	constexpr inline operator T() const noexcept
	{
		return static_cast<T>(m_value) / m_factor;
	}

	// Conversion to an integral type
	template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	constexpr inline operator T() const noexcept
	{
		return static_cast<T>(m_value / m_factor);
	}

	constexpr inline operator BaseType() const noexcept
	{
		return m_value;
	}
};

// Detect if a type is a COM interface (Direct or Indirect Inheritance from IUnknown)
template <typename T, typename = void>
struct is_com_interface : std::false_type {};

template <typename T>
struct is_com_interface<T, std::enable_if_t<std::is_base_of_v<IUnknown, std::remove_pointer_t<T>>>> : std::true_type {};

// Detect if a type is a ComPtr holding an IUnknown-derived object
template <typename T>
struct is_com_ptr : std::false_type {};

template <typename T>
struct is_com_ptr<Microsoft::WRL::ComPtr<T>> : is_com_interface<T> {};

// Overload 1: **Raw Pointers**
template <typename T>
std::enable_if_t<!is_com_interface<T>::value&& std::is_pointer_v<T>>
SAFE_RELEASE(T& ptr) {
	if (ptr) {
		try {
			delete ptr;
			ptr = nullptr;
			LOG(trace) << "[SAFE_RELEASE] Raw Pointer delete successful.";
		}
		catch (const std::exception& e) {
			LOG(error) << "[SAFE_RELEASE] Exception while deleting raw pointer: " << e.what();
		}
		catch (...) {
			LOG(error) << "[SAFE_RELEASE] Unknown exception occurred while deleting raw pointer!";
		}
	}
}

// Overload 2: **Raw COM Pointers**
template <typename T>
std::enable_if_t<is_com_interface<T>::value&& std::is_pointer_v<T>>
SAFE_RELEASE(T& ptr) {
	if (ptr) {
		try {
			ULONG refCount = ptr->Release();
			ptr = nullptr;
			if (refCount > 0)
			{
				LOG(debug) << "[SAFE_RELEASE] COM object released, remaining ref count: " << refCount;
			}
			else
			{
				LOG(trace) << "[SAFE_RELEASE] COM object released, remaining ref count: " << refCount;
			}
		}
		catch (const std::exception& e) {
			LOG(error) << "[SAFE_RELEASE] Exception while releasing COM object: " << e.what();
		}
		catch (...) {
			LOG(error) << "[SAFE_RELEASE] Unknown exception occurred while releasing COM object!";
		}
	}
}

// Overload 3: **ComPtr<T>**
template <typename T>
std::enable_if_t<is_com_ptr<T>::value>
SAFE_RELEASE(T& ptr) {
	try {
		ptr.Reset();
		LOG(trace) << "[SAFE_RELEASE] ComPtr Reset successful.";
	}
	catch (const std::exception& e) {
		LOG(error) << "[SAFE_RELEASE] Exception while resetting ComPtr: " << e.what();
	}
	catch (...) {
		LOG(error) << "[SAFE_RELEASE] Unknown exception occurred while resetting ComPtr!";
	}
}

// Overload 4: **unique_ptr<T> and shared_ptr<T>**
template <typename T>
void SAFE_RELEASE(std::unique_ptr<T>& ptr) {
	try {
		ptr.reset();
		LOG(trace) << "[SAFE_RELEASE] unique_ptr reset successful.";
	}
	catch (const std::exception& e) {
		LOG(error) << "[SAFE_RELEASE] Exception while resetting unique_ptr: " << e.what();
	}
	catch (...) {
		LOG(error) << "[SAFE_RELEASE] Unknown exception occurred while resetting unique_ptr!";
	}
}

template <typename T>
void SAFE_RELEASE(std::shared_ptr<T>& ptr) {
	try {
		ptr.reset();
		LOG(trace) << "[SAFE_RELEASE] shared_ptr reset successful.";
	}
	catch (const std::exception& e) {
		LOG(error) << "[SAFE_RELEASE] Exception while resetting shared_ptr: " << e.what();
	}
	catch (...) {
		LOG(error) << "[SAFE_RELEASE] Unknown exception occurred while resetting shared_ptr!";
	}
}

static sz::string FormatHResultMessage(const HRESULT hr) {
	try {
		wchar_t* msgBuffer = nullptr;
		DWORD size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&msgBuffer, 0, nullptr);

		if (size == 0 || msgBuffer == nullptr) {
			return "Unknown error (FormatMessageW failed)";
		}

		sz::string message = ToString(msgBuffer);
		LocalFree(msgBuffer);
		return message;

	}
	catch (const std::exception& e) {
		LOG(error) << "Exception in FormatHResultMessage: " << e.what();
		return "Unknown error (Exception occurred)";
	}
}

// Function to round up to the nearest multiple of alignment
static size_t AlignUp(const size_t size, const size_t alignment) {
	return (size + alignment - 1) & ~(alignment - 1);
}

#endif // UTILITY_HPP_
