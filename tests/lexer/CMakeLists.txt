cmake_minimum_required(VERSION 3.22.1)

project(ctc_lexer_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(DEBUG "Compile with debug symbols and without optimization" OFF)
option(BUILD_SHARED_LIB "Build as shared library(default: runtime is static library)" OFF)

set(LIB_PATH "../../src/lexer/")

include_directories(
    ${LIB_PATH}
)

if (BUILD_SHARED_LIB)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

if(DEBUG)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-g3 -O0)

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/Zi /Od)

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-g -O0)

    endif()

endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options(-std=c++20 -Wall -Wextra -Wpedantic)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   add_compile_options(/W4 /std:c++20)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options(-Weverything -std=c++20)

endif()

add_library(
ctc_compiler ${LIB_TYPE}
    ${LIB_PATH}lexer.h
    ${LIB_PATH}token.h

    ${LIB_PATH}token.cpp
    ${LIB_PATH}lexer.cpp
)

add_executable(
ctc_lexer_tests
    lexer_tests.cpp
)

target_link_libraries(ctc_lexer_tests ctc_compiler)
