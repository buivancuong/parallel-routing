//
// Created by cuongbv on 29/02/2020.
//

#include <iostream>
#include "../../graph/fattree/FatTree.h"
#include "../../node/FatTreeNode.h"

int main() {
    int paramK = 4;
    auto *fatTree = new FatTree(paramK);
    std::cout << "create fat tree topo " << fatTree->getNumVertices() << " node" << std::endl;
    int numCoreSwitch = fatTree->getNumCoreSwitch();
    int numPods = fatTree->getNumPods();
    int numSwitchPerPod = fatTree->getNumSwitchPerPod();
    std::map<int, FatTreeNode*> fatTreeNodeList;

    for (int i = 0; i < fatTree->getNumVertices(); ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }

    for (std::pair<int, FatTreeNode*> fatTreeNode : fatTreeNodeList) {
        fatTreeNode.second->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
    std::cout << "Done" << std::endl;

    return 0;
}