file(GLOB Files "*.hpp" "*.cpp")

add_executable("ComsEngine-Test" "${Files}")

target_link_libraries("ComsEngine-Test" PUBLIC "gtest")
target_link_libraries("ComsEngine-Test" PUBLIC "ComsEngine-Script")

add_test("test" "ComsEngine-Test")