file(GLOB Files "*.hpp" "*.cpp")

add_library("ComsEngine-Base" STATIC "${Files}")

target_include_directories("ComsEngine-Base" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")