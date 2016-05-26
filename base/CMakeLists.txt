file(GLOB Files *.hpp *.cpp)

add_library(Base STATIC ${Files})

target_include_directories(Base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})