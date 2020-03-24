cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(parallel_routing main.cpp
        graph/Graph.cpp graph/Graph.h
        routing/RoutingAlgorithm.h routing/DijkstraAlgorithm.cpp routing/DijkstraAlgorithm.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h graph/smallworld/SmallWorld2DTorus.cpp graph/smallworld/SmallWorld2DTorus.h
        routing/CORRADistributed.cpp routing/CORRADistributed.h
        node/Node.cpp node/Node.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h
        node/Edge.cpp node/Edge.h
        graph/fattree/FatTree.cpp graph/fattree/FatTree.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h
        routing/FatTreeRoutingAlgorithm.cpp routing/FatTreeRoutingAlgorithm.h
        node/CORRANode.cpp node/CORRANode.h node/TZNode.cpp node/TZNode.h node/FatTreeNode.cpp node/FatTreeNode.h
        exp/serial/CORRAv1SExp.cpp exp/serial/CORRAv1SExp.h exp/parallel/CORRAv1PExp.cpp exp/parallel/CORRAv1PExp.h exp/serial/CORRAv2SExp.cpp exp/serial/CORRAv2SExp.h exp/parallel/CORRAv2PExp.cpp exp/parallel/CORRAv2PExp.h exp/serial/TZSExp.cpp exp/serial/TZSExp.h utils/TZUtils.cpp utils/TZUtils.h exp/parallel/TZPExp.cpp exp/parallel/TZPExp.h exp/serial/FatTreeSExp.cpp exp/serial/FatTreeSExp.h exp/parallel/FatTreePExp.cpp exp/parallel/FatTreePExp.h)
add_executable(corra_v1_serial experiment/corra_v1/corra_v1_serial.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/CORRANode.cpp node/CORRANode.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)
add_executable(corra_v1_parallel experiment/corra_v1/corra_v1_parallel.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/CORRANode.cpp node/CORRANode.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h experiment/corra_v1/corra_v1_exp_serial.cpp experiment/corra_v1/corra_v1_exp_parallel.cpp)
add_executable(corra_v2_serial experiment/corra_v2/corra_v2_serial.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/Node.cpp node/Node.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)
add_executable(corra_v2_parallel experiment/corra_v2/corra_v2_parallel.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/Node.cpp node/Node.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h experiment/corra_v2/corra_v2_exp_serial.cpp experiment/corra_v2/corra_v2_exp_parallel.cpp)
add_executable(tz_serial experiment/tz/tz_serial.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/TZNode.cpp node/TZNode.h
        utils/TZUtils.cpp utils/TZUtils.h)
add_executable(tz_parallel experiment/tz/tz_parallel.cpp
        graph/Graph.cpp graph/Graph.h
            graph/smallworld/GridGraph.cpp graph/smallworld/GridGraph.h
            graph/smallworld/SmallWorld2DGrid.cpp graph/smallworld/SmallWorld2DGrid.h
        node/TZNode.cpp node/TZNode.h
        utils/TZUtils.cpp utils/TZUtils.h experiment/tz/tz_exp_serial.cpp)
add_executable(fat_tree_serial experiment/fat_tree/fat_tree_serial.cpp
        graph/Graph.cpp graph/Graph.h
            graph/fattree/FatTree.cpp graph/fattree/FatTree.h
        node/FatTreeNode.cpp node/FatTreeNode.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h)
add_executable(fat_tree_parallel experiment/fat_tree/fat_tree_parallel.cpp
        graph/Graph.cpp graph/Graph.h
            graph/fattree/FatTree.cpp graph/fattree/FatTree.h
        node/FatTreeNode.cpp node/FatTreeNode.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h)

add_executable(main main.cpp)