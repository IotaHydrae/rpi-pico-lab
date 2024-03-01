add_library(factory_test INTERFACE)

target_include_directories(factory_test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

file(GLOB_RECURSE FONT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/font/*.c)

target_sources(factory_test INTERFACE ${SOURCES} ${FONT_SOURCES})

target_link_libraries(factory_test INTERFACE
    pico_stdlib
    pico_bootrom
    pico_unique_id
    hardware_clocks
)