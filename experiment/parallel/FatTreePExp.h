//
// Created by cuongbv on 22/02/2020.
//

#ifndef PARALLEL_ROUTING_FATTREEPEXP_H
#define PARALLEL_ROUTING_FATTREEPEXP_H


#include <map>
#include "../../node/FatTreeNode.h"

class FatTreePExp {
private:
    std::map<int, FatTreeNode*> fatTreeNodeList;

public:

    void createFatTreeNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);

    void buildTable(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);

};


#endif //PARALLEL_ROUTING_FATTREEPEXP_H
