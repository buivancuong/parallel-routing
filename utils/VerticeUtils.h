//
// Created by cuongbv on 23/10/2019.
//

#ifndef PARALLEL_ROUTING_VERTICEUTILS_H
#define PARALLEL_ROUTING_VERTICEUTILS_H


#include <set>
#include "../graph_entities/Vertice.h"

class VerticeUtils {
public:
    VerticeUtils();
    ~VerticeUtils();

    // method to get horizontal of the vertice
    static int getVerticeHorizontal(int verticeID, int xTopoSize);
    // method to get vertical of the vertice
    static int getVerticeVertical(int verticeID, int yTopoSize);
    // method to get block ID of the vertice
    static int getVerticeBlock(int verticeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    // method to get horizontal of the block
    static int getBlockHorizontal(int blockId, int xBlockSize, int xTopoSize);
    // method to get vertical of the block
    static int getBlockVertical(int blockID, int yBlockSize, int yTopoSize);
    // method to get center vertice of the block
    static int getCenterVertice(int blockId, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    // method to get max hop-distance to all nodes on same block of the node
    static int getMaxHopinBlock(int verticeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
};


#endif //PARALLEL_ROUTING_VERTICEUTILS_H
