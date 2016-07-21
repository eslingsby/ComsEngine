file(GLOB Files "*.hpp" "*.cpp")

add_library("ComsEngine-Default" STATIC "${Files}")

target_link_libraries("ComsEngine-Default" LINK_PUBLIC "ComsEngine-Base")

target_link_libraries("ComsEngine-Default" LINK_PUBLIC "SDL2")
target_link_libraries("ComsEngine-Default" LINK_PUBLIC "glm")

target_include_directories("ComsEngine-Default" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")