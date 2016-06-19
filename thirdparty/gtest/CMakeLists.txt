file(GLOB_RECURSE Files *.h *.cc LICENSE)

add_library(_GTest STATIC ${Files})

target_include_directories(_GTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})