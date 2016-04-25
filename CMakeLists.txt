# -*- indent-tabs-mode: nil; -*-
# vim: ts=4:sts=0:sw=4:et

cmake_minimum_required(VERSION 3.3.1)

set(CMAKE_CXX_COMPILER "/usr/bin/g++53")

project("rns")

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_RULE_MESSAGES OFF)

set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard version.")
add_compile_options(-Wall -Wextra -Werror -Wno-missing-field-initializers)
add_compile_options(-pedantic-errors -D_GLIBCXX_USE_CXX11_ABI=0)
add_compile_options(-gdwarf-2)
add_compile_options(-O2)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS) # remove -rdynamic

include_directories("include")

add_subdirectory("src")
add_subdirectory("tests")
