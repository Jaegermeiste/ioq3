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

/*
quake3 common
2025 Bleeding Eye Studios
*/

#ifndef STDAFX_H_
#define STDAFX_H_
#pragma once

#pragma system_header

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#pragma runtime_checks( "", off )
#pragma warning(push,0)

#pragma warning(disable:26498)
#pragma warning(disable:26495)
#pragma warning(disable:26439)
#pragma warning(disable:26827)
#pragma warning(disable:4146)
#pragma warning(disable:4068)


// STL Includes
#include <string>
#include <sstream>
#include <any>
#include <format>
#include <iomanip>
#include <memory>
#include <mutex>
#include <atomic>
#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <concepts>
#include <typeinfo>
#include <type_traits>
#include <codecvt>
#include <utility>
#include <fstream>

// Other
#include <stringzilla/stringzilla.hpp>

namespace sz = ashvardanian::stringzilla;

#define MAGIC_ENUM_RANGE_MAX 256
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

#include <simdutf.h>
#include <simdutf/implementation.h>

//#include <unicode/utypes.h>
//#include <unicode/utf8.h>
//#include <unicode/uchar.h>
//#include <unicode/unistr.h>
//#include <unicode/brkiter.h>

//#include <fribidi/fribidi.h>

/*#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H  // for variable fonts
#include FT_TRUETYPE_TABLES_H  // TT_OS2

#include <harfbuzz/hb-cplusplus.hh>
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-ot.h>*/

// Boost Includes
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE	1
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/locale.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/filesystem.hpp>
#include <boost/endian.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>
#include <boost/circular_buffer.hpp>

// Boost Container Index Tags
struct name {};
struct handle {};
struct registration {};

// Windows Includes
#include <windows.h>
#include <mmsystem.h>
#include <Shlobj_core.h>
//#include <d3d12.h>
//#include <d3dx12.h>
//#include <dxgi1_6.h>
//#include <D3Dcompiler.h>
//#include <dxcapi.h>
//#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
#include <wrl.h>
#include <shellapi.h>
#include <SafeInt.h>
#include <ppl.h>
#include <Urlmon.h>
#include <D2d1_1.h>
#include <Dwrite.h>
#include <comdef.h>

using Microsoft::WRL::ComPtr;
/*
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "DXGI.lib")

// DirectXTK12
#include <DirectXTK12/CommonStates.h>
#include <DirectXTK12/DDSTextureLoader.h>
#include <DirectXTK12/DirectXHelpers.h>
#include <DirectXTK12/Effects.h>
#include <DirectXTK12/GamePad.h>
#include <DirectXTK12/GeometricPrimitive.h>
#include <DirectXTK12/GraphicsMemory.h>
#include <DirectXTK12/Keyboard.h>
#include <DirectXTK12/Model.h>
#include <DirectXTK12/Mouse.h>
#include <DirectXTK12/PostProcess.h>
#include <DirectXTK12/PrimitiveBatch.h>
#include <DirectXTK12/ResourceUploadBatch.h>
#include <DirectXTK12/ScreenGrab.h>
#include <DirectXTK12/SimpleMath.h>
#include <DirectXTK12/SpriteBatch.h>
#include <DirectXTK12/SpriteFont.h>
#include <DirectXTK12/VertexTypes.h>
#include <DirectXTK12/WICTextureLoader.h>

//#include "d3dx12.h"

// DirectXTex
#include <DirectXTex.h>
#include <DirectXTexEXR.h>

// Microsoft GDK
#include <xCurl.h>*/

// Sound
#include <mmdeviceapi.h>
#include <SpatialAudioClient.h>

#include <Functiondiscoverykeys_devpkey.h>

// Steam Audio
#include <phonon.h>

#include <minizip-ng/mz.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

#include <physfs.h>
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//using namespace DirectX::PackedVector;

/*template <>
struct magic_enum::customize::enum_range<D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAGS> {
	static constexpr bool is_flags = true;
};

template <>
struct magic_enum::customize::enum_range<D3D12_COMMAND_LIST_SUPPORT_FLAGS> {
	static constexpr bool is_flags = true;
};

template <>
struct magic_enum::customize::enum_range<D3D12_SHADER_CACHE_SUPPORT_FLAGS> {
	static constexpr bool is_flags = true;
};*/

//https://stackoverflow.com/questions/20104815/warning-c4316-object-allocated-on-the-heap-may-not-be-aligned-16	
#define ALIGNED_16_MEMORY_OPERATORS										\
void*	operator new	(size_t i)	{	return _mm_malloc(i, 16);	}	\
void	operator delete	(void* p)	{	_mm_free(p);	}

// Define a concept to check if T is derived from Parent
template<typename T, typename Parent>
concept DerivedFrom = std::is_base_of_v<Parent, T>;

template<typename T, typename U>
concept SameType = std::is_same_v<T, U>;

template<typename T, typename U>
auto CompareSharedPtrTypes(const std::shared_ptr<T>& ptr1, const std::shared_ptr<U>& ptr2) {
	if constexpr (SameType<T, U>) {
		return true;
	}
	else {
		return false;
	}
}

#pragma runtime_checks( "", restore )
#pragma warning(pop)

#endif//STDAFX_H_