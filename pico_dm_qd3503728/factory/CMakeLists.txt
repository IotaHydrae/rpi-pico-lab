add_library(factory_test INTERFACE)

target_include_directories(factory_test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

target_sources(factory_test INTERFACE ${SOURCES})