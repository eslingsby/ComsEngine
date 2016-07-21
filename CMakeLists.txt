cmake_minimum_required(VERSION 2.6)

project("ComsEngine" CXX)

if(MSVC)
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_SOURCE_DIR}/bin")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_SOURCE_DIR}/bin/lib")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_SOURCE_DIR}/bin/lib")
    endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
else(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
endif(MSVC)

function("add_external_tar" folder url include)
	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}/${include}")
		file(DOWNLOAD "${url}" "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}.tar.gz")

		execute_process(
			COMMAND "${CMAKE_COMMAND}" -E tar xzf "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}.tar.gz"
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty"
		)

		file(REMOVE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}.tar.gz")
	endif()

	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${folder}/${include}")
endfunction("add_external_tar")

add_external_tar("SDL2-2.0.4" "https://www.libsdl.org/release/SDL2-2.0.4.tar.gz" "include")
add_external_tar("glm-0.9.7.6" "https://github.com/g-truc/glm/archive/0.9.7.6.tar.gz" "")
add_external_tar("lua-5.3.3" "https://www.lua.org/ftp/lua-5.3.3.tar.gz" "src")
add_external_tar("googletest-release-1.7.0" "https://github.com/google/googletest/archive/release-1.7.0.tar.gz" "include")

add_subdirectory("base")
add_subdirectory("default")
add_subdirectory("script")

add_subdirectory("main")

enable_testing()
add_subdirectory("test")