file(GLOB Files "*.hpp" "*.cpp")

add_library("ComsEngine-Default" STATIC "${Files}")

target_link_libraries("ComsEngine-Default" PUBLIC "ComsEngine-Base")

target_link_libraries("ComsEngine-Default" PUBLIC "SDL2")
target_link_libraries("ComsEngine-Default" PUBLIC "SDL2main")
target_link_libraries("ComsEngine-Default" PUBLIC "glm")
target_link_libraries("ComsEngine-Default" PUBLIC "glew_s")

target_include_directories("ComsEngine-Default" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
