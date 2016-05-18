file(GLOB SRCS *.hpp *.cpp)

add_executable(app ${SRCS})

target_link_libraries(app LINK_PUBLIC base)