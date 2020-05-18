//
// Created by cuongbv on 22/02/2020.
//

#include "FatTreeSExp.h"
#include "../../graph/fattree/FatTree.h"
#include "../../node/FatTreeNode.h"

FatTreeSExp::FatTreeSExp(int paramK) {
    auto *fatTree = new FatTree(paramK);
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
}

FatTreeSExp::~FatTreeSExp() = default;
