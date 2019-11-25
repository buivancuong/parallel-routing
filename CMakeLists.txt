cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_routing main.cpp graph/Graph.cpp graph/Graph.h)