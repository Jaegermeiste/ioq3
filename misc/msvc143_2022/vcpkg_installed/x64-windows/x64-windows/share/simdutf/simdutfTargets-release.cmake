#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "simdutf::simdutf" for configuration "Release"
set_property(TARGET simdutf::simdutf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(simdutf::simdutf PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/simdutf.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/simdutf.dll"
  )

list(APPEND _cmake_import_check_targets simdutf::simdutf )
list(APPEND _cmake_import_check_files_for_simdutf::simdutf "${_IMPORT_PREFIX}/lib/simdutf.lib" "${_IMPORT_PREFIX}/bin/simdutf.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
