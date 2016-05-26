file(GLOB Files *.hpp *.cpp)

add_executable(Test ${Files})

target_link_libraries(Test LINK_PUBLIC Base)