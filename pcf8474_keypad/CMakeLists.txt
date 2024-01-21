if (NOT TARGET pcf8574_keypad)
    add_library(pcf8574_keypad INTERFACE)

    target_sources(pcf8574_keypad INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/i2c_tools.c
            ${CMAKE_CURRENT_LIST_DIR}/keypad.c
            ${CMAKE_CURRENT_LIST_DIR}/pcf8574.c
    )

    target_include_directories(pcf8574_keypad INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries(pcf8574_keypad INTERFACE
        pico_stdlib
        hardware_i2c
    )
endif()