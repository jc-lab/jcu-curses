add_executable(test-app main.cc)

target_link_libraries(test-app
        jcu_curses
        ${CURSES_LIBRARIES}
        )
