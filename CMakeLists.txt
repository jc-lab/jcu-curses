cmake_minimum_required(VERSION 3.12)
project(jcu-curses CXX)

include(FetchContent)

option(jcu_curses_BUILD_TESTS "Build tests" OFF)

# ==================== CURSES LIBRARY ====================

find_package(Curses REQUIRED)

# ==================== SOURCE ====================

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(SRC_FILES
        ${SRC_DIR}/screen.cc
        ${SRC_DIR}/window.cc
        ${SRC_DIR}/box_window.cc
        ${SRC_DIR}/progress.cc
        ${SRC_DIR}/utils.h
        ${SRC_DIR}/utils.cc
        )

set(INC_FILES
        ${INC_DIR}/jcu-curses/types.h
        ${INC_DIR}/jcu-curses/screen.h
        ${INC_DIR}/jcu-curses/window.h
        ${INC_DIR}/jcu-curses/box_window.h
        ${INC_DIR}/jcu-curses/progress.h
        )

add_library(jcu_curses STATIC ${SRC_FILES} ${INC_FILES})
target_include_directories(jcu_curses
        INTERFACE
        ${INC_DIR}
        PUBLIC
        ${CURSES_INCLUDE_DIRS}
        ${INC_DIR}
        )

target_link_libraries(jcu_curses
        PRIVATE
        pthread
        ${CURSES_LIBRARIES}
)

# ================================================

if (jcu_curses_BUILD_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "")

    if (NOT TARGET gtest)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG release-1.10.0
        )

        FetchContent_GetProperties(googletest)

        if (NOT googletest_POPULATED)
            FetchContent_Populate(googletest)
            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
        endif ()
    endif ()

    enable_testing()
    add_subdirectory(test)
endif ()

add_subdirectory(test)
