cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set (BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(libtuxnbits++
    VERSION 0.1.0
    DESCRIPTION "A non-standard c++ framework suite of my personal projects"
    LANGUAGES CXX)

OPTION(BITS         "Build the the bits and bytes support...........[libtuxnbits++.a]"     ON)
OPTION(IO           "Build the IO sublib............................[libtuxnbits++.io.a]"  ON)
OPTION(LEXER        "Build the lexer sublib.........................[libtuxnbits++.lexer.a]" OFF)
OPTION(EST          "Build the est (Expression Syntax Tree) sublib..[libtuxnbits++.est.a]" OFF)
OPTION(TUI          "Build the console ANSI UI  sublib..............[libtuxnbits++.tui.a]" OFF)
OPTION(TESTS        "Build the tests................................[bitstest/bits.app]" ON)


if(BITS)
    message(STATUS "Building libtunbits++.bits.a")
    include(bits.cmake)
endif()

if(IO)
    message(STATUS "Building libobb.io.a")
    include(io.cmake)

endif()

if(LEXER)
    message(STATUS "Building libobb.lexer.a")
    include(obb.lexer.cmake)
endif()

if(EST)
    message(STATUS "Building libobb.est.a")
    include(obb.est.cmake)
endif()

if(TUI)
    message(STATUS "Building libobb.tui.a")
    add_subdirectory(tuxnbits++/bits/tui)
endif()

if(TESTS)
    message(STATUS "framework tests:")
    include(bitstest.cmake)
endif()
