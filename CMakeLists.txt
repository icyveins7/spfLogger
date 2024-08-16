cmake_minimum_required(VERSION 3.20)

project(spfLogger)

# Set C++ standard to C++20, for source_location
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build example
add_executable(log example/log.cpp)
target_include_directories(log PUBLIC include/)

# Build test


