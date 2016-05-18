file(GLOB SRCS *.hpp *.cpp)

add_library(base STATIC ${SRCS})

target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})