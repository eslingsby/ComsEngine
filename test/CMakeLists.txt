file(GLOB Files "*.hpp" "*.cpp")

add_executable("ComsEngine-Test" "${Files}")

target_link_libraries("ComsEngine-Test" LINK_PUBLIC "gtest")

target_link_libraries("ComsEngine-Test" LINK_PUBLIC "ComsEngine-Script")

enable_testing()

add_test("test" "ComsEngine-Test")