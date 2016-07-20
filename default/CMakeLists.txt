file(GLOB Files *.hpp *.cpp)

add_library(Default STATIC ${Files})

target_link_libraries(Default LINK_PUBLIC Base)

target_link_libraries(Default LINK_PUBLIC _Lua)
target_link_libraries(Default LINK_PUBLIC _GLM)

target_include_directories(Default PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})