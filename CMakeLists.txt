cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(parallel_routing main.cpp graph/Graph.cpp graph/Graph.h routing/RoutingAlgorithm.h routing/DijkstraAlgorithm.cpp routing/DijkstraAlgorithm.h graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h graph/smallworld/SmallWorld2DTorus.cpp graph/smallworld/SmallWorld2DTorus.h routing/CORRADistributed.cpp routing/CORRADistributed.h node/Node.cpp node/Node.h utils/CORRAUtils.cpp utils/CORRAUtils.h node/Edge.cpp node/Edge.h experiment/CORRADistributedExperiment.cpp experiment/CORRADistributedExperiment.h graph/fattree/FatTree.cpp graph/fattree/FatTree.h utils/FatTreeUtils.cpp utils/FatTreeUtils.h routing/FatTreeRoutingAlgorithm.cpp routing/FatTreeRoutingAlgorithm.h node/CORRANode.cpp node/CORRANode.h)