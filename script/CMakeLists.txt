file(GLOB Files "*.hpp" "*.cpp")

add_library("ComsEngine-Script" STATIC "${Files}")

target_link_libraries("ComsEngine-Script" PUBLIC "ComsEngine-Default")
target_link_libraries("ComsEngine-Script" PUBLIC "lua")

target_include_directories("ComsEngine-Script" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")