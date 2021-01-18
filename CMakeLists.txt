cmake_minimum_required(VERSION 3.12)

include(CMakePackageConfigHelpers)

SET(lib_version 1.0)

project(libKIni VERSION ${lib_version} 
				DESCRIPTION "A library for editing KromeIni files"
				LANGUAGES CXX)
				
add_subdirectory(src)

write_basic_package_version_file(
	libKIniConfigVersion.cmake
	VERSION ${lib_version} 
	COMPATIBILITY AnyNewerVersion)
	
install(TARGETS KIni 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib 
		INCLUDES DESTINATION include
		FILE libKIniConfig.cmake
		NAMESPACE libKIni::
		DESTINATION lib/cmake/libKIni)
	
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libKIniConfig.cmake"
			  "${CMAKE_CURRENT_BINARY_DIR}/libKIniConfigVersion.cmake"
		DESTINATION lib/cmake/libKIni)