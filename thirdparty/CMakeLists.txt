function("add_external_tar" name url include)
	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.tar.gz" )
		file(DOWNLOAD "${url}" "${CMAKE_CURRENT_SOURCE_DIR}/${name}.tar.gz")

		execute_process(
			COMMAND "${CMAKE_COMMAND}" -E tar xzf "${CMAKE_CURRENT_SOURCE_DIR}/${name}.tar.gz"
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		)
	endif()

	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${name}")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${name}/${include}")
endfunction("add_external_tar")

add_external_tar("SDL2-2.0.4" "https://www.libsdl.org/release/SDL2-2.0.4.tar.gz" "include")

add_external_tar("glm-0.9.7.6" "https://github.com/g-truc/glm/archive/0.9.7.6.tar.gz" "")

add_external_tar("lua-5.3.3" "https://www.lua.org/ftp/lua-5.3.3.tar.gz" "src")

add_external_tar("googletest-release-1.7.0" "https://github.com/google/googletest/archive/release-1.7.0.tar.gz" "include")