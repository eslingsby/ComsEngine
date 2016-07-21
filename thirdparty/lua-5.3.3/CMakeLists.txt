file(GLOB_RECURSE Files "*.h" "*.c")

add_library("lua" STATIC "${Files}")

target_include_directories("lua" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")