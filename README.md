# The Framework is used to develop/simulate step-by-step on Routing Table construction, workflow of algorithms on Interconnection Network.

## 1. Graph framework and scaling.

In ```./graph/```:
* ```./graph/fattree/``` is the object which simulate the graph of Fat Tree topo.
* ```./graph/smallworld/``` is the object which simulate the graph of Smallworld 2D Grid or Smallworld 2D Torus
We can add a new topology on here.

In ```./utils``` is the utilities for the above topo (Fat Tree, Smallworld).

In ```./routing``` is the Routing Algorithms.

## 2. Node object.

On here, we have some node object which correspond with the routing algorithm.

* ```CORRANode``` ~ CORRA v1 Algorithm.
* ```Node``` ~ CORRA v2 Algorithm.
* ```TZNode``` ~ Compact routing Algorithm (Thorup - Zwick).
* ```FatTreeNode``` ~ Fat Tree routing Algorithm.

## 3. Multiple experiment executable program and Cmake 

Add new executable experiment file, we edit on CmakeLists.txt

#### Fat Tree 

To build parallel experiment program:

```$cmake
add_executable(fat_tree_parallel experiment/fat_tree/fat_tree_parallel.cpp
        graph/Graph.cpp graph/Graph.h
            graph/fattree/FatTree.cpp graph/fattree/FatTree.h
        node/FatTreeNode.cpp node/FatTreeNode.h
        utils/FatTreeUtils.cpp utils/FatTreeUtils.h)
```

* ```fat_tree_parallel``` is the name of parallel experiment program, it will appear on ```./cmake-build-debug/```.
* ```experiment/fat_tree/fat_tree_parallel.cpp``` is the path of source code cpp experiment file.
* These following path files is the neccessary source code file for the parallel experiment program.

## 4. Read more on ```./buglog.md```