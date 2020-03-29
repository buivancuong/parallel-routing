cmake_minimum_required(VERSION 3.15)
project(parallel_routing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

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
        utils/CORRAUtils.cpp utils/CORRAUtils.h)

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
        utils/CORRAUtils.cpp utils/CORRAUtils.h )

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
        utils/TZUtils.cpp utils/TZUtils.h)

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

add_executable(corra_v1_exp_serial experiment/corra_v1/corra_v1_exp_serial.cpp
        graph/Graph.cpp graph/Graph.h
        node/CORRANode.cpp node/CORRANode.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)

add_executable(corra_v1_exp_parallel experiment/corra_v1/corra_v1_exp_parallel.cpp
        graph/Graph.cpp graph/Graph.h
        node/CORRANode.cpp node/CORRANode.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)

add_executable(corra_v2_exp_serial experiment/corra_v2/corra_v2_exp_serial.cpp
        node/Node.cpp node/Node.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)

add_executable(corra_v2_exp_parallel experiment/corra_v2/corra_v2_exp_parallel.cpp
        node/Node.cpp node/Node.h
        utils/CORRAUtils.cpp utils/CORRAUtils.h)

add_executable(tz_exp_serial experiment/tz/tz_exp_serial.cpp
        graph/Graph.cpp graph/Graph.h
        node/TZNode.cpp node/TZNode.h)

add_executable(tz_exp_parallel experiment/tz/tz_exp_parallel.cpp
        graph/Graph.cpp graph/Graph.h
        node/TZNode.cpp node/TZNode.h)

add_executable(fat_tree_exp_serial experiment/fat_tree/fat_tree_exp_serial.cpp
        node/FatTreeNode.cpp node/FatTreeNode.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h)

add_executable(fat_tree_exp_parallel experiment/fat_tree/fat_tree_exp_parallel.cpp
        node/FatTreeNode.cpp node/FatTreeNode.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h)

add_executable(main main.cpp)