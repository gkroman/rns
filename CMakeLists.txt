# -*- indent-tabs-mode: nil; -*-
# vim: ts=4:sts=0:sw=4:et

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "/usr/bin/g++53")

project("rns")

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_RULE_MESSAGES OFF)

set(LANGSTD "-std=c++14")
set(OPTFLAGS "-O3")
set(DEBUG "-gdwarf-2")
set(WARNINGS "-Wall -Wextra -Werror -Wno-missing-field-initializers -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LANGSTD} ${OPTFLAGS} ${DEBUG} ${WARNINGS}")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS) # remove -rdynamic

if (NOT DEFINED PREFIX)
    set(PREFIX "${PROJECT_BINARY_DIR}")
endif()
set(CMAKE_INSTALL_PREFIX ${PREFIX})

include_directories("include")

add_subdirectory("src")
add_subdirectory("tests")
