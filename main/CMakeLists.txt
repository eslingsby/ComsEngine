file(GLOB Files "*.hpp" "*.cpp")

add_executable("ComsEngine-Main" "${Files}")

target_link_libraries("ComsEngine-Main" PUBLIC "ComsEngine-Script")