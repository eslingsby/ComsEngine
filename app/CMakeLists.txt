file(GLOB Files "*.hpp" "*.cpp")

add_executable("ComsEngine-Main" "${Files}")

target_link_libraries("ComsEngine-Main" LINK_PUBLIC "ComsEngine-Script")