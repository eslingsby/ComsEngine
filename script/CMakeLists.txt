file(GLOB Files *.hpp *.cpp)

add_library(Script STATIC ${Files})

target_link_libraries(Script LINK_PUBLIC Base)

target_link_libraries(Script LINK_PUBLIC _Lua)
target_link_libraries(Script LINK_PUBLIC _GLM)

target_include_directories(Script PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})