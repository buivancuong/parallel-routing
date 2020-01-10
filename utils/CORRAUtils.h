//
// Created by cuongbv on 20/12/2019.
//

#ifndef PARALLEL_ROUTING_CORRAUTILS_H
#define PARALLEL_ROUTING_CORRAUTILS_H


#include <set>

class CORRAUtils {
public:
    CORRAUtils();
    ~CORRAUtils();

    // method to get horizontal of the vertice
    static int getNodeHorizontal(int nodeID, int xTopoSize);
    // method to get vertical of the vertice
    static int getNodeVertical(int nodeID, int xTopoSize);
    // method to get block ID of the vertice
    static int getNodeBlock(int nodeID, int xBlockSize, int yBlockSize, int xTopoSize);
    // method to get distance follow grid-hop between 2 vertex
    static int getGridHop(int sourceID, int destID, int xTopoSize);
    // method to get horizontal of the block
    static int getBlockHorizontal(int blockId, int xBlockSize, int xTopoSize);
    // method to get vertical of the block
    static int getBlockVertical(int blockID, int xBlockSize, int xTopoSize);
    // method to get center vertice of the block
    static int getCenterVertex(int blockId, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    // method to get max hop-distance to all nodes on same block of the node
    static int getMaxHopinBlock(int nodeID, int xBlockSize, int yBlockSize, int xTopoSize);
};


#endif //PARALLEL_ROUTING_CORRAUTILS_H
