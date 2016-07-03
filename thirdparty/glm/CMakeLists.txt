file(GLOB_RECURSE Files *.hpp *.cpp *.inl LICENSE)

add_library(_GLM STATIC ${Files})

target_include_directories(_GLM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})