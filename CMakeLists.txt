cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(parallel_routing main.cpp graph/Graph.cpp graph/Graph.h routing/RoutingAlgorithm.h routing/DijkstraAlgorithm.cpp routing/DijkstraAlgorithm.h graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h graph/smallworld/SmallWorld2DTorus.cpp graph/smallworld/SmallWorld2DTorus.h routing/CORRADistributed.cpp routing/CORRADistributed.h node/Node.cpp node/Node.h utils/CORRAUtils.cpp utils/CORRAUtils.h node/Edge.cpp node/Edge.h graph/fattree/FatTree.cpp graph/fattree/FatTree.h utils/FatTreeUtils.cpp utils/FatTreeUtils.h routing/FatTreeRoutingAlgorithm.cpp routing/FatTreeRoutingAlgorithm.h node/CORRANode.cpp node/CORRANode.h node/TZNode.cpp node/TZNode.h node/FatTreeNode.cpp node/FatTreeNode.h experiment/serial/CORRAv1SExp.cpp experiment/serial/CORRAv1SExp.h experiment/parallel/CORRAv1PExp.cpp experiment/parallel/CORRAv1PExp.h experiment/serial/CORRAv2SExp.cpp experiment/serial/CORRAv2SExp.h experiment/parallel/CORRAv2PExp.cpp experiment/parallel/CORRAv2PExp.h experiment/serial/TZSExp.cpp experiment/serial/TZSExp.h utils/TZUtils.cpp utils/TZUtils.h experiment/parallel/TZPExp.cpp experiment/parallel/TZPExp.h experiment/serial/FatTreeSExp.cpp experiment/serial/FatTreeSExp.h experiment/parallel/FatTreePExp.cpp experiment/parallel/FatTreePExp.h)