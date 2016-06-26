file(GLOB_RECURSE Files *.h *.c LICENSE)

add_library(_Lua STATIC ${Files})

target_include_directories(_Lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})