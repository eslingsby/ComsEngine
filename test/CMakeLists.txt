file(GLOB Files *.hpp *.cpp)

add_executable(Test ${Files})

target_link_libraries(Test LINK_PUBLIC _GTest)

target_link_libraries(Test LINK_PUBLIC Script)

enable_testing()

add_test(Tester Test)