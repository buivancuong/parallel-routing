cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_routing main.cpp graph_entities/Vertice.cpp graph_entities/Vertice.h graph_entities/Edge.cpp graph_entities/Edge.h graph_entities/Graph.cpp graph_entities/Graph.h utils/VerticeUtils.cpp utils/VerticeUtils.h)