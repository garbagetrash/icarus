cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(icarus VERSION 1.0.0 LANGUAGES CXX)

# Enable tests
enable_testing()

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local)
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

add_library(icarus
    src/diff_corr.cpp
)
target_include_directories(icarus PUBLIC include)
target_link_libraries(icarus PUBLIC rt)

install(TARGETS icarus)
