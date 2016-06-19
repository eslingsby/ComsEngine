file(GLOB Files *.hpp *.cpp)

add_executable(App ${Files})

target_link_libraries(App LINK_PUBLIC Script)