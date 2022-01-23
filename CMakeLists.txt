cmake_minimum_required(VERSION 3.21)
project(epq)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(epq src/main.cpp src/icg.cpp src/ipd.cpp src/utility.cpp)
