cmake_minimum_required(VERSION 3.22.1)

project(ctc LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(DEBUG "Compile with debug symbols and without optimization" OFF)
option(BUILD_SHARED_LIB "Build as shared library(default: runtime is static library)" OFF)

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

include_directories(${PROJECT_SOURCE_DIR}/src)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
