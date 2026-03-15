#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "reactor-cpp-default" for configuration "Debug"
set_property(TARGET reactor-cpp-default APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(reactor-cpp-default PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/reactor-cpp-default.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/reactor-cpp-default.dll"
  )

list(APPEND _cmake_import_check_targets reactor-cpp-default )
list(APPEND _cmake_import_check_files_for_reactor-cpp-default "${_IMPORT_PREFIX}/lib/reactor-cpp-default.lib" "${_IMPORT_PREFIX}/bin/reactor-cpp-default.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
