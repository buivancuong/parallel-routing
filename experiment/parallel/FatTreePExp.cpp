//
// Created by cuongbv on 22/02/2020.
//

#include "FatTreePExp.h"

void FatTreePExp::createFatTreeNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        this->fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }
}

void FatTreePExp::buildTable(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->fatTreeNodeList[i]->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
}
